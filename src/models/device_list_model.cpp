#include "device_list_model.h"
#include <QDateTime>

DeviceListModel::DeviceListModel(QObject *parent) : QAbstractTableModel(parent)
{
}

int DeviceListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_devices.size();
}

int DeviceListModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return Column::ColumnCount;
}

QVariant DeviceListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_devices.size())
        return QVariant();

    const NetworkDevice &device = m_devices[index.row()];

    if (role == Qt::DisplayRole) {
        switch (static_cast<Column>(index.column())) {
            case Column::IpAddress:
                return device.getIpAddress().toString();
            case Column::MacAddress:
                return device.getMacAddress();
            case Column::Hostname:
                return device.getHostname();
            case Column::Status:
                return device.getStatusString();
            case Column::LastSeen:
                return device.getLastSeen().toString("yyyy-MM-dd hh:mm:ss");
            case Column::ResponseTime:
                return device.getResponseTime() >= 0 ? 
                    QString::number(device.getResponseTime()) + " ms" : "N/A";
            default:
                return QVariant();
        }
    } else if (role == Qt::TextAlignmentRole) {
        return static_cast<int>(Qt::AlignLeft | Qt::AlignVCenter);
    }

    return QVariant();
}

QVariant DeviceListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        return columnName(static_cast<Column>(section));
    }

    return QVariant();
}

void DeviceListModel::setDevices(const QVector<NetworkDevice> &devices)
{
    beginResetModel();
    m_devices = devices;
    endResetModel();
}

void DeviceListModel::addDeviceToModel(const NetworkDevice &device)
{
    beginInsertRows(QModelIndex(), m_devices.size(), m_devices.size());
    m_devices.append(device);
    endInsertRows();
}

void DeviceListModel::removeDeviceFromModel(const QHostAddress &ip)
{
    for (int i = 0; i < m_devices.size(); ++i) {
        if (m_devices[i].getIpAddress() == ip) {
            beginRemoveRows(QModelIndex(), i, i);
            m_devices.remove(i);
            endRemoveRows();
            return;
        }
    }
}

void DeviceListModel::updateDeviceInModel(const NetworkDevice &device)
{
    for (int i = 0; i < m_devices.size(); ++i) {
        if (m_devices[i].getIpAddress() == device.getIpAddress()) {
            m_devices[i] = device;
            QModelIndex topLeft = index(i, 0);
            QModelIndex bottomRight = index(i, columnCount() - 1);
            emit dataChanged(topLeft, bottomRight);
            return;
        }
    }
}

void DeviceListModel::clearModel()
{
    beginResetModel();
    m_devices.clear();
    endResetModel();
}

NetworkDevice DeviceListModel::getDeviceAt(const QModelIndex &index) const
{
    if (!index.isValid() || index.row() >= m_devices.size())
        return NetworkDevice();
    return m_devices[index.row()];
}

NetworkDevice DeviceListModel::getDeviceByIp(const QHostAddress &ip) const
{
    auto it = std::find_if(m_devices.begin(), m_devices.end(),
        [&ip](const NetworkDevice &device) {
            return device.getIpAddress() == ip;
        });
    return it != m_devices.end() ? *it : NetworkDevice();
}

QString DeviceListModel::columnName(Column column)
{
    switch (column) {
        case Column::IpAddress:
            return QStringLiteral("IP Address");
        case Column::MacAddress:
            return QStringLiteral("MAC Address");
        case Column::Hostname:
            return QStringLiteral("Hostname");
        case Column::Status:
            return QStringLiteral("Status");
        case Column::LastSeen:
            return QStringLiteral("Last Seen");
        case Column::ResponseTime:
            return QStringLiteral("Response Time");
        default:
            return QString();
    }
}