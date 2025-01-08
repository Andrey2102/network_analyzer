#include "device_list_model.h"
#include <QDateTime>

DeviceListModel::DeviceListModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

int DeviceListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_filterString.isEmpty() ? m_devices.size() : m_filteredDevices.size();
}

int DeviceListModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return Column::ColumnCount;
}

QVariant DeviceListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    const NetworkDevice &device = m_filterString.isEmpty() ? 
        m_devices[index.row()] : m_filteredDevices[index.row()];

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

void DeviceListModel::addDevice(const NetworkDevice &device)
{
    // Check if device already exists
    auto it = std::find_if(m_devices.begin(), m_devices.end(),
        [&device](const NetworkDevice &existing) {
            return existing.getIpAddress() == device.getIpAddress();
        });

    if (it != m_devices.end()) {
        updateDevice(device);
        return;
    }

    beginInsertRows(QModelIndex(), m_devices.size(), m_devices.size());
    m_devices.append(device);
    endInsertRows();

    updateFilteredDevices();
}

void DeviceListModel::updateDevice(const NetworkDevice &device)
{
    for (int i = 0; i < m_devices.size(); ++i) {
        if (m_devices[i].getIpAddress() == device.getIpAddress()) {
            m_devices[i] = device;
            QModelIndex topLeft = index(i, 0);
            QModelIndex bottomRight = index(i, columnCount() - 1);
            emit dataChanged(topLeft, bottomRight);
            updateFilteredDevices();
            return;
        }
    }
}

void DeviceListModel::removeDevice(const QHostAddress &ipAddress)
{
    for (int i = 0; i < m_devices.size(); ++i) {
        if (m_devices[i].getIpAddress() == ipAddress) {
            beginRemoveRows(QModelIndex(), i, i);
            m_devices.remove(i);
            endRemoveRows();
            updateFilteredDevices();
            return;
        }
    }
}

void DeviceListModel::clear()
{
    beginResetModel();
    m_devices.clear();
    m_filteredDevices.clear();
    endResetModel();
}

NetworkDevice DeviceListModel::getDevice(const QModelIndex &index) const
{
    if (!index.isValid() || index.row() >= m_devices.size())
        return NetworkDevice();
    
    return m_filterString.isEmpty() ? 
        m_devices[index.row()] : m_filteredDevices[index.row()];
}

NetworkDevice DeviceListModel::getDeviceByIp(const QHostAddress &ipAddress) const
{
    auto it = std::find_if(m_devices.begin(), m_devices.end(),
        [&ipAddress](const NetworkDevice &device) {
            return device.getIpAddress() == ipAddress;
        });
    
    return it != m_devices.end() ? *it : NetworkDevice();
}

QVector<NetworkDevice> DeviceListModel::getAllDevices() const
{
    return m_devices;
}

void DeviceListModel::setFilterString(const QString &filter)
{
    if (m_filterString != filter) {
        m_filterString = filter;
        updateFilteredDevices();
        emit layoutChanged();
    }
}

void DeviceListModel::updateFilteredDevices()
{
    if (m_filterString.isEmpty()) {
        m_filteredDevices.clear();
        return;
    }

    m_filteredDevices.clear();
    for (const auto &device : m_devices) {
        if (deviceMatchesFilter(device)) {
            m_filteredDevices.append(device);
        }
    }
}

bool DeviceListModel::deviceMatchesFilter(const NetworkDevice &device) const
{
    return device.getIpAddress().toString().contains(m_filterString, Qt::CaseInsensitive) ||
           device.getMacAddress().contains(m_filterString, Qt::CaseInsensitive) ||
           device.getHostname().contains(m_filterString, Qt::CaseInsensitive);
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