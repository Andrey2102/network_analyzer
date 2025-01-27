#include "network_scanner.h"
#include <QHostInfo>
#include <QProcess>
#include <QDebug>
#include <QNetworkInterface>
#include <QRegularExpression>
#include <QFile>
#include <QTextStream>
#include <QtConcurrent>
#include <QThreadPool>

NetworkScanner* NetworkScanner::_network_scanner = nullptr;

NetworkScanner::NetworkScanner() : 
    QObject(nullptr),
    _isScanning(false),
    _currentProgress(0),
    lastReportedProgress(0),
    _maxThreads(16),
    _runningThreads(0)
{
    updateNetworkInfo();
}

NetworkScanner::~NetworkScanner() {
    stopScan();
    if (_network_scanner == this) {
        _network_scanner = nullptr;
    }
}

NetworkScanner* NetworkScanner::getInstance() {
    if (_network_scanner == nullptr) {
        _network_scanner = new NetworkScanner();
    }
    return _network_scanner;
}

void NetworkScanner::updateNetworkInfo() {
    startIp = QHostAddress("192.168.1.1");
    endIp = QHostAddress("192.168.1.255");
}

void NetworkScanner::startScan(const QString& ipRange) {
    // Check if a scan is already in progress
    if (_isScanning) {
        return;
    }

    // Initialize scan state
    _isScanning = true;
    _currentProgress = 0;
    lastReportedProgress = 0;

    // Clear the list of devices
    {
        QMutexLocker locker(&_mutex);
        _devices.clear();
    }

    // Step 1: Check ARP cache for devices that have recently communicated
    checkArpCache();

    // Step 2: Generate a list of IP addresses to scan
    QVector<QHostAddress> addresses;
    quint32 start = startIp.toIPv4Address(); // Start of IP range (e.g., 192.168.1.1)
    quint32 end = endIp.toIPv4Address();     // End of IP range (e.g., 192.168.1.255)

    // Populate the list with all IPs in the range
    for (quint32 i = start; i <= end; ++i) {
        addresses.append(QHostAddress(i));
    }

    // Step 3: Divide the IP addresses into batches for multi-threaded scanning
    int batchSize = qMax(1, addresses.size() / _maxThreads); // Calculate batch size
    _runningThreads = 0; // Reset the running threads counter

    // Process each batch in a separate thread
    for (int i = 0; i < addresses.size(); i += batchSize) {
        QVector<QHostAddress> batch = addresses.mid(i, qMin(batchSize, addresses.size() - i));
        _runningThreads++; // Increment the running threads counter

        // Start a new thread to scan the batch
        QThreadPool::globalInstance()->start([this, batch]() {
            scanBatch(batch); // Scan the batch of IPs
            QMetaObject::invokeMethod(this, "handleBatchFinished", Qt::QueuedConnection); // Notify when done
        });
    }
}

void NetworkScanner::scanBatch(const QVector<QHostAddress>& batch) {
    for (const QHostAddress& ip : batch) {
        if (!_isScanning) return;

        // Step 1: Ping the device
        QProcess pingProcess;
        QStringList arguments;

#ifdef Q_OS_LINUX
        arguments << "-c" << "1" << "-W" << "1" << ip.toString();
#else
        arguments << "-n" << "1" << "-w" << "1000" << ip.toString();
#endif

        pingProcess.start("ping", arguments);
        bool isAlive = pingProcess.waitForFinished(1000) && pingProcess.exitCode() == 0;

        // Step 2: If the device responds to ping, add it to the list
        if (isAlive) {
            NetworkDevice device{ip};
            device.setStatus(NetworkDevice::Status::Online);
            device.setLastSeen(QDateTime::currentDateTime());

            // Step 3: Get the MAC address using ARP
            QString mac = getMacAddress(ip);
            if (!mac.isEmpty()) {
                device.setMacAddress(mac);
            }

            // Step 4: Add the device to the list (thread-safe)
            {
                QMutexLocker locker(&_mutex);
                if (!deviceExists(device.getIpAddress())) {
                    _devices.append(device);
                    emit deviceFound(device); // Notify the UI
                }
            }

            // Step 5: Get the hostname using QHostInfo
            QHostInfo::lookupHost(ip.toString(), this, [this, ip](const QHostInfo& hostInfo) {
                if (hostInfo.error() == QHostInfo::NoError) {
                    QString hostname = hostInfo.hostName();

                    // Update the device with the hostname
                    QMutexLocker locker(&_mutex);
                    for (NetworkDevice& device : _devices) {
                        if (device.getIpAddress() == ip) {
                            device.setHostname(hostname);
                            emit deviceFound(device); // Notify the UI
                            break;
                        }
                    }
                }
            });
        }

        // Step 6: Update the scan progress (thread-safe)
        {
            QMutexLocker locker(&_mutex);
            _currentProgress++;
            int totalIps = endIp.toIPv4Address() - startIp.toIPv4Address() + 1;
            int progress = (_currentProgress * 100) / totalIps;
            if (progress != lastReportedProgress) {
                lastReportedProgress = progress;
                emit scanProgress(progress); // Notify the UI
            }
        }
    }
}

void NetworkScanner::handleHostLookup(const QHostInfo& info) {
    if (info.error() != QHostInfo::NoError) return;

    QMutexLocker locker(&_mutex);
    for (int i = 0; i < _devices.size(); ++i) {
        if (_devices[i].getIpAddress().toString() == info.addresses().first().toString()) {
            _devices[i].setHostname(info.hostName());
            emit deviceFound(_devices[i]);
            break;
        }
    }
}

void NetworkScanner::handleBatchFinished() {
    QMutexLocker locker(&_mutex);
    _runningThreads--;
    
    if (_runningThreads == 0) {
        _isScanning = false;
        emit scanFinished();
    }
}

void NetworkScanner::checkArpCache() {
    QFile arpCache("/proc/net/arp"); // Path to ARP cache on Linux
    if (!arpCache.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QTextStream in(&arpCache);
    QString line = in.readLine(); // Skip the header line

    // Parse each line in the ARP cache
    while (!in.atEnd()) {
        line = in.readLine();
        QStringList fields = line.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);

        // Check if the line contains valid ARP data
        if (fields.size() >= 4 && fields[2] == "0x2") { // 0x2 means "reachable"
            QHostAddress ip(fields[0]); // IP address
            QString mac = fields[3];    // MAC address

            // Create a device and add it to the list
            NetworkDevice device{ip};
            device.setMacAddress(mac);
            device.setStatus(NetworkDevice::Status::Online);
            device.setLastSeen(QDateTime::currentDateTime());

            {
                QMutexLocker locker(&_mutex);
                if (!deviceExists(device.getIpAddress())) {
                    _devices.append(device);
                    emit deviceFound(device); // Notify the UI
                }
            }
        }
    }
}

QString NetworkScanner::getMacAddress(const QHostAddress& ip) {
    QProcess arpProcess;

#ifdef Q_OS_LINUX
    arpProcess.start("arp", QStringList() << "-n" << ip.toString()); // Linux arp command
#else
    arpProcess.start("arp", QStringList() << "-a" << ip.toString()); // Windows arp command
#endif

    if (!arpProcess.waitForFinished(500)) {
        arpProcess.kill();
        return QString();
    }

    // Parse the output to find the MAC address
    QString output = QString::fromLocal8Bit(arpProcess.readAllStandardOutput());
    QRegularExpression macRegex("([0-9A-Fa-f]{2}[:-]){5}([0-9A-Fa-f]{2})");
    QRegularExpressionMatch match = macRegex.match(output);

    return match.hasMatch() ? match.captured(0) : QString();
}

bool NetworkScanner::deviceExists(const QHostAddress& ip) {
    return std::any_of(_devices.begin(), _devices.end(),
        [&ip](const NetworkDevice& device) {
            return device.getIpAddress() == ip;
        });
}

void NetworkScanner::stopScan() {
    _isScanning = false;
}

QVector<NetworkDevice> NetworkScanner::getDevices() const {
    QMutexLocker locker(&_mutex);
    return _devices;
}

void NetworkScanner::scanNetwork() {
    quint32 start = startIp.toIPv4Address();
    quint32 end = endIp.toIPv4Address();

    for (quint32 i = start; i <= end; ++i) {
        if (!_isScanning) break;

        QHostAddress ip(i);

        // Ping устройство
        QProcess pingProcess;
        pingProcess.start("ping", QStringList() << "-c" << "1" << "-W" << "1" << ip.toString());

        bool isAlive = pingProcess.waitForFinished(1000) && pingProcess.exitCode() == 0;

        if (isAlive) {
            // Устройство ответило на ping
            NetworkDevice device{ip};
            device.setStatus(NetworkDevice::Status::Online);
            device.setLastSeen(QDateTime::currentDateTime());

            // Получаем MAC-адрес через ARP
            QString mac = getMacAddress(ip);
            if (!mac.isEmpty()) {
                device.setMacAddress(mac);
            }

            {
                QMutexLocker locker(&_mutex);
                if (!deviceExists(device.getIpAddress())) {
                    _devices.append(device);
                    emit deviceFound(device);
                }
            }
        }

        {
            QMutexLocker locker(&_mutex);
            _currentProgress++;
            int totalIps = end - start + 1;
            int progress = (_currentProgress * 100) / totalIps;
            if (progress != lastReportedProgress) {
                lastReportedProgress = progress;
                emit scanProgress(progress);
            }
        }
    }
}