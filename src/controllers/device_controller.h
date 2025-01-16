#ifndef DEVICE_CONTROLLER_H
#define DEVICE_CONTROLLER_H

#include <QObject>
#include <QHostAddress>
#include "../models/device_list_model.h"
#include "../models/network_device.h"

class DeviceController : public QObject
{
    Q_OBJECT

public:
    explicit DeviceController(QObject *parent = nullptr);
    ~DeviceController();

    // CRUD operations
    void addDevice(const NetworkDevice &device);
    void updateDevice(const NetworkDevice &device);
    void removeDevice(const QHostAddress &ipAddress);
    void clearDevices();

    // Export operations
    void exportToCsv(const QString &filePath);
    void exportToJson(const QString &filePath);

    // Getters
    DeviceListModel* getModel() const { return m_model; }
    NetworkDevice getDeviceByIp(const QHostAddress &ip) const;
    QVector<NetworkDevice> getAllDevices() const;

signals:
    void deviceAdded(const NetworkDevice &device);
    void deviceUpdated(const NetworkDevice &device);
    void deviceRemoved(const QHostAddress &ipAddress);
    void devicesCleared();
    void exportFinished(bool success, const QString &message);

private:
    DeviceListModel *m_model;
};

#endif // DEVICE_CONTROLLER_H