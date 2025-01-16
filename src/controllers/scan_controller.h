#ifndef SCAN_CONTROLLER_H
#define SCAN_CONTROLLER_H

#include <QObject>
#include "device_controller.h"
#include "../services/network_scanner.h"

class ScanController : public QObject
{
    Q_OBJECT

public:
    explicit ScanController(DeviceController *deviceController, QObject *parent = nullptr);
    ~ScanController();

    void startScan();
    void stopScan();
    void pingDevice(const QHostAddress &ipAddress);

signals:
    void scanStarted();
    void scanFinished();
    void scanProgress(int progress);
    void scanError(const QString &error);
    void pingResult(const QHostAddress &ipAddress, bool success, int responseTime);

private slots:
    void handleDeviceFound(const NetworkDevice &device);
    void handleScanError(const QString &error);
    void handleScanComplete();

private:
    DeviceController *m_deviceController;
    NetworkScanner *m_networkScanner;
};

#endif // SCAN_CONTROLLER_H