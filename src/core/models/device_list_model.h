#ifndef DEVICE_LIST_MODEL_H
#define DEVICE_LIST_MODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include "../network_device.h"

class DeviceListModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    // Column enumeration for better code readability
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

    // Required QAbstractTableModel overrides
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Device management methods
    void addDevice(const NetworkDevice &device);
    void updateDevice(const NetworkDevice &device);
    void removeDevice(const QHostAddress &ipAddress);
    void clear();
    
    // Device access methods
    NetworkDevice getDevice(const QModelIndex &index) const;
    NetworkDevice getDeviceByIp(const QHostAddress &ipAddress) const;
    QVector<NetworkDevice> getAllDevices() const;

    // Filtering methods
    void setFilterString(const QString &filter);

private:
    QVector<NetworkDevice> m_devices;
    QVector<NetworkDevice> m_filteredDevices;
    QString m_filterString;

    void updateFilteredDevices();
    bool deviceMatchesFilter(const NetworkDevice &device) const;
    static QString columnName(Column column);
};

#endif // DEVICE_LIST_MODEL_H