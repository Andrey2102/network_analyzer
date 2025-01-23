#ifndef NETWORK_SCANNER_H
#define NETWORK_SCANNER_H

#include <QObject>
#include <QVector>
#include <QHostAddress>
#include "network_device.h"

class QProcess;

class NetworkScanner : public QObject {
    Q_OBJECT

private:
    NetworkScanner();
    static NetworkScanner* _network_scanner;
    
    QVector<NetworkDevice> _devices;
    QHostAddress _startIp;
    QHostAddress _endIp;
    QHostAddress _currentIp;
    bool _isScanning;
    int _currentProgress;
    QProcess* _process;

    void scanNextAddress();
    void pingHost(const QHostAddress& ip);
    QString getMacAddress(const QHostAddress& ip);

private slots:
    void handlePingResult(int exitCode);

public:
    ~NetworkScanner();
    static NetworkScanner* getInstance();
    
    void startScan(const QString& ipRange);
    void stopScan();
    QVector<NetworkDevice> getDevices() const;

    NetworkScanner(const NetworkScanner&) = delete;
    NetworkScanner& operator=(const NetworkScanner&) = delete;

signals:
    void deviceFound(const NetworkDevice& device);
    void scanProgress(int progress);
    void scanFinished();
    void scanError(const QString& error);
};

#endif // NETWORK_SCANNER_H