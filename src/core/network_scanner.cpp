#include "network_scanner.h"
#include <QHostInfo>
#include <QProcess>
#include <QThread>
#include <QDebug>
#include <QNetworkInterface>
#include <QRegularExpression>

NetworkScanner* NetworkScanner::_network_scanner = nullptr;

NetworkScanner::NetworkScanner() : 
    QObject(nullptr),
    _isScanning(false),
    _currentProgress(0)
{
    _process = new QProcess(this);
    connect(_process, &QProcess::finished, this, &NetworkScanner::handlePingResult);
}

NetworkScanner::~NetworkScanner() {
    stopScan();
    delete _process;
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

void NetworkScanner::startScan(const QString& ipRange) {
    if (_isScanning) {
        return;
    }

    _isScanning = true;
    _currentProgress = 0;
    _devices.clear();

    _startIp = QHostAddress("192.168.1.1");
    _endIp = QHostAddress("192.168.1.255");

    scanNextAddress();
}

void NetworkScanner::scanNextAddress() {
    if (!_isScanning) {
        emit scanFinished();
        return;
    }

    quint32 current = _currentIp.isNull() ? 
        _startIp.toIPv4Address() : 
        _currentIp.toIPv4Address() + 1;

    if (current > _endIp.toIPv4Address()) {
        _isScanning = false;
        emit scanFinished();
        return;
    }

    _currentIp = QHostAddress(current);
    pingHost(_currentIp);
}

void NetworkScanner::pingHost(const QHostAddress& ip) {
    QStringList arguments;

#ifdef Q_OS_LINUX
    arguments << "-c" << "1" << "-W" << "1" << ip.toString();
#else
    arguments << "-n" << "1" << "-w" << "1000" << ip.toString();
#endif

    _process->start("ping", arguments);
}

void NetworkScanner::handlePingResult(int exitCode) {
    if (exitCode == 0) {
        NetworkDevice device(_currentIp);
        device.setHostname(_currentIp.toString());
        device.setStatus(NetworkDevice::Status::Online);
        device.setLastSeen(QDateTime::currentDateTime());

        QString mac = getMacAddress(_currentIp);
        if (!mac.isEmpty()) {
            device.setMacAddress(mac);
        }

        _devices.append(device);
        emit deviceFound(device);
    }

    int progress = ((_currentIp.toIPv4Address() - _startIp.toIPv4Address()) * 100) /
                  (_endIp.toIPv4Address() - _startIp.toIPv4Address());
    
    if (progress != _currentProgress) {
        _currentProgress = progress;
        emit scanProgress(_currentProgress);
    }

    scanNextAddress();
}

QString NetworkScanner::getMacAddress(const QHostAddress& ip) {
    QProcess arpProcess;
    arpProcess.start("arp", QStringList() << "-n" << ip.toString());
    arpProcess.waitForFinished();

    QString output = QString::fromLocal8Bit(arpProcess.readAllStandardOutput());
    QRegularExpression macRegex("([0-9A-Fa-f]{2}[:-]){5}([0-9A-Fa-f]{2})");
    QRegularExpressionMatch match = macRegex.match(output);
    
    return match.hasMatch() ? match.captured(0) : QString();
}

void NetworkScanner::stopScan() {
    _isScanning = false;
    if (_process->state() == QProcess::Running) {
        _process->kill();
    }
}

QVector<NetworkDevice> NetworkScanner::getDevices() const {
    return _devices;
}