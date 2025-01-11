#ifndef DEVICE_LIST_MODEL_H
#define DEVICE_LIST_MODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include "network_device.h"

class DeviceListModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    // Columns enum
    enum Column {
        IpAddress = 0,
        MacAddress,
        Hostname,
        Status,
        LastSeen,
        ResponseTime,
        ColumnCount
    };

    explicit DeviceListModel(QObject *parent = nullptr);

    // QAbstractTableModel required overrides
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Internal methods for data manipulation (used by controller)
    void setDevices(const QVector<NetworkDevice> &devices);
    void addDeviceToModel(const NetworkDevice &device);
    void removeDeviceFromModel(const QHostAddress &ip);
    void updateDeviceInModel(const NetworkDevice &device);
    void clearModel();
    
    // Data access methods
    NetworkDevice getDeviceAt(const QModelIndex &index) const;
    NetworkDevice getDeviceByIp(const QHostAddress &ip) const;
    const QVector<NetworkDevice>& getDevices() const { return m_devices; }

private:
    QVector<NetworkDevice> m_devices;
    static QString columnName(Column column);
};

#endif // DEVICE_LIST_MODEL_H