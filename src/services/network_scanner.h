#ifndef _NETWORK_SCANNER_H_
#define _NETWORK_SCANNER_H_

#include <QObject>
#include <QVector>
#include "models/network_device.h"

// Make it inherit from QObject to support signals
class NetworkScanner : public QObject
{
    Q_OBJECT

signals:
    // Signals that scan controller will listen to
    void deviceFound(const NetworkDevice &device);
    void scanComplete();
    void scanError(const QString &error);
    void progressUpdated(int progress);

public:
    // Static method to get the instance
    static NetworkScanner* getInstance();

    // Scanning methods
    void scanCurrentDevice();
    void startScan();
    void stopScan();
    void pingDevice(const QHostAddress &address);

private:
    QVector<NetworkDevice> m_devices;
    bool m_isScanning;

    // Private constructor for singleton
    NetworkScanner(QObject *parent = nullptr);
    ~NetworkScanner();
    
    // Delete copy constructor and assignment operator
    NetworkScanner(const NetworkScanner&) = delete;
    NetworkScanner& operator=(const NetworkScanner&) = delete;

    // Private methods
    QString getActiveLocalInterfaceName();

    // Static instance
    static NetworkScanner* m_instance;
};

#endif // _NETWORK_SCANNER_H_