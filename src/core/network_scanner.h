#ifndef NETWORK_SCANNER_H
#define NETWORK_SCANNER_H

#include <QObject>
#include <QVector>
#include <QHostAddress>
#include <QHostInfo>
#include <QMutex>
#include "network_device.h"

class QProcess;

class NetworkScanner : public QObject {
    Q_OBJECT

private:
    NetworkScanner();
    static NetworkScanner* _network_scanner;
    
    QVector<NetworkDevice> _devices;
    bool _isScanning;
    int _currentProgress;
    int lastReportedProgress;
    const int _maxThreads;
    int _runningThreads;
    mutable QMutex _mutex;
    QHostAddress startIp;
    QHostAddress endIp;

    void scanBatch(const QVector<QHostAddress>& batch);
    QString getMacAddress(const QHostAddress& ip);
    void updateNetworkInfo();
    void checkArpCache();
    bool deviceExists(const QHostAddress& ip);

private slots:
    void handleBatchFinished();
    void handleHostLookup(const QHostInfo& info);

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