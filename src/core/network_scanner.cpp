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
    if (_isScanning) {
        return;
    }

    _isScanning = true;
    _currentProgress = 0;
    lastReportedProgress = 0;

    {
        QMutexLocker locker(&_mutex);
        _devices.clear();
    }

    checkArpCache();

    QVector<QHostAddress> addresses;
    quint32 start = startIp.toIPv4Address();
    quint32 end = endIp.toIPv4Address();

    for (quint32 i = start; i <= end; ++i) {
        addresses.append(QHostAddress(i));
    }

    int batchSize = qMax(1, addresses.size() / _maxThreads);
    _runningThreads = 0;

    for (int i = 0; i < addresses.size(); i += batchSize) {
        QVector<QHostAddress> batch = addresses.mid(i, qMin(batchSize, addresses.size() - i));
        _runningThreads++;

        QThreadPool::globalInstance()->start([this, batch]() {
            scanBatch(batch);
            QMetaObject::invokeMethod(this, "handleBatchFinished", Qt::QueuedConnection);
        });
    }
}

void NetworkScanner::scanBatch(const QVector<QHostAddress>& batch) {
    for (const QHostAddress& ip : batch) {
        if (!_isScanning) return;

        QProcess process;
        QStringList arguments;

#ifdef Q_OS_LINUX
        arguments << "-c" << "1" << "-W" << "1" << ip.toString();
#else
        arguments << "-n" << "1" << "-w" << "1000" << ip.toString();
#endif

        process.start("ping", arguments);
        bool isAlive = process.waitForFinished(1000) && process.exitCode() == 0;

        if (isAlive) {
            NetworkDevice device(ip);
            device.setStatus(NetworkDevice::Status::Online);
            device.setLastSeen(QDateTime::currentDateTime());
            device.setHostname(ip.toString());

            QString mac = getMacAddress(ip);
            if (!mac.isEmpty()) {
                device.setMacAddress(mac);
            }

            {
                QMutexLocker locker(&_mutex);
                if (!deviceExists(ip)) {
                    _devices.append(device);
                    emit deviceFound(device);
                }
            }

            QHostInfo::lookupHost(ip.toString(), this, SLOT(handleHostLookup(QHostInfo)));
        }

        {
            QMutexLocker locker(&_mutex);
            _currentProgress++;
            int totalIps = endIp.toIPv4Address() - startIp.toIPv4Address() + 1;
            int progress = (_currentProgress * 100) / totalIps;
            if (progress != lastReportedProgress) {
                lastReportedProgress = progress;
                emit scanProgress(progress);
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
    QFile arpCache("/proc/net/arp");
    if (!arpCache.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QTextStream in(&arpCache);
    QString line = in.readLine(); // Skip header

    while (!in.atEnd()) {
        line = in.readLine();
        QStringList fields = line.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
        
        if (fields.size() >= 4 && fields[2] == "0x2") {
            QHostAddress ip(fields[0]);
            if (!deviceExists(ip)) {
                NetworkDevice device(ip);
                device.setMacAddress(fields[3]);
                device.setHostname(fields[0]);
                device.setStatus(NetworkDevice::Status::Online);
                device.setLastSeen(QDateTime::currentDateTime());
                _devices.append(device);
                emit deviceFound(device);
            }
        }
    }
}

QString NetworkScanner::getMacAddress(const QHostAddress& ip) {
    QProcess arpProcess;
    
#ifdef Q_OS_LINUX
    arpProcess.start("arp", QStringList() << "-n" << ip.toString());
#else
    arpProcess.start("arp", QStringList() << "-a" << ip.toString());
#endif

    if (!arpProcess.waitForFinished(500)) {
        arpProcess.kill();
        return QString();
    }

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