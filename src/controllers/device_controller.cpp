#include "device_controller.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QTextStream>

DeviceController::DeviceController(QObject *parent)
    : QObject(parent)
    , m_model(new DeviceListModel(this))
{
}

DeviceController::~DeviceController()
{
}

void DeviceController::addDevice(const NetworkDevice &device)
{
    m_model->addDeviceToModel(device);
    emit deviceAdded(device);
}

void DeviceController::updateDevice(const NetworkDevice &device)
{
    m_model->updateDeviceInModel(device);
    emit deviceUpdated(device);
}

void DeviceController::removeDevice(const QHostAddress &ipAddress)
{
    m_model->removeDeviceFromModel(ipAddress);
    emit deviceRemoved(ipAddress);
}

void DeviceController::clearDevices()
{
    m_model->clearModel();
    emit devicesCleared();
}

NetworkDevice DeviceController::getDeviceByIp(const QHostAddress &ip) const
{
    return m_model->getDeviceByIp(ip);
}

QVector<NetworkDevice> DeviceController::getAllDevices() const
{
    return m_model->getDevices();
}

void DeviceController::exportToCsv(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        emit exportFinished(false, "Could not open file for writing");
        return;
    }

    QTextStream out(&file);
    out << "IP,MAC,Hostname,Status,Last Seen,Response Time\n";

    for (const NetworkDevice &device : m_model->getDevices()) {
        out << device.getIpAddress().toString() << ","
            << device.getMacAddress() << ","
            << device.getHostname() << ","
            << device.getStatusString() << ","
            << device.getLastSeen().toString("yyyy-MM-dd hh:mm:ss") << ","
            << device.getResponseTime() << "\n";
    }

    file.close();
    emit exportFinished(true, "Export to CSV completed successfully");
}

void DeviceController::exportToJson(const QString &filePath)
{
    QJsonArray deviceArray;

    for (const NetworkDevice &device : m_model->getDevices()) {
        QJsonObject deviceObject;
        deviceObject["ip"] = device.getIpAddress().toString();
        deviceObject["mac"] = device.getMacAddress();
        deviceObject["hostname"] = device.getHostname();
        deviceObject["status"] = device.getStatusString();
        deviceObject["lastSeen"] = device.getLastSeen().toString("yyyy-MM-dd hh:mm:ss");
        deviceObject["responseTime"] = device.getResponseTime();
        
        deviceArray.append(deviceObject);
    }

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        emit exportFinished(false, "Could not open file for writing");
        return;
    }

    QJsonDocument document(deviceArray);
    file.write(document.toJson());
    file.close();

    emit exportFinished(true, "Export to JSON completed successfully");
}