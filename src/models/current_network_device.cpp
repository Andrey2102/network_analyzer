#include "current_network_device.h"

#include <string>

CurrentNetworkDevice::CurrentNetworkDevice()
    : m_interfaceName("N/A")
    , m_ipAddress("N/A")
    , m_macAddress("N/A")
    , m_netmask("N/A")
    , m_broadcastAddress("N/A")
{
}

CurrentNetworkDevice::CurrentNetworkDevice(const QString& interfaceName, const QString& ipAddress,
                                             const QString& macAddress, const QString& netmask,
                                             const QString& broadcastAddress)
    : m_interfaceName(interfaceName)
    , m_ipAddress(ipAddress)
    , m_macAddress(macAddress)
    , m_netmask(netmask)
    , m_broadcastAddress(broadcastAddress)
{
}

CurrentNetworkDevice::CurrentNetworkDevice(const CurrentNetworkDevice& other)
    : m_interfaceName(other.m_interfaceName)
    , m_ipAddress(other.m_ipAddress)
    , m_macAddress(other.m_macAddress)
    , m_netmask(other.m_netmask)
    , m_broadcastAddress(other.m_broadcastAddress)
{
}

CurrentNetworkDevice& CurrentNetworkDevice::operator=(const CurrentNetworkDevice& other)
{
    if (this != &other) {
        m_interfaceName = other.m_interfaceName;
        m_ipAddress = other.m_ipAddress;
        m_macAddress = other.m_macAddress;
        m_netmask = other.m_netmask;
        m_broadcastAddress = other.m_broadcastAddress;
    }
    return *this;
}

CurrentNetworkDevice::CurrentNetworkDevice(CurrentNetworkDevice&& other) noexcept
    : m_interfaceName(std::move(other.m_interfaceName))
    , m_ipAddress(std::move(other.m_ipAddress))
    , m_macAddress(std::move(other.m_macAddress))
    , m_netmask(std::move(other.m_netmask))
    , m_broadcastAddress(std::move(other.m_broadcastAddress))
{
}

CurrentNetworkDevice& CurrentNetworkDevice::operator=(CurrentNetworkDevice&& other) noexcept
{
    if (this != &other) {
        m_interfaceName = std::move(other.m_interfaceName);
        m_ipAddress = std::move(other.m_ipAddress);
        m_macAddress = std::move(other.m_macAddress);
        m_netmask = std::move(other.m_netmask);
        m_broadcastAddress = std::move(other.m_broadcastAddress);
    }
    return *this;
}
CurrentNetworkDevice::~CurrentNetworkDevice()
{
    // Destructor implementation (if needed)
}
QString CurrentNetworkDevice::getInterfaceName() const
{
    return m_interfaceName;
}
QString CurrentNetworkDevice::getIpAddress() const
{
    return m_ipAddress;
}
QString CurrentNetworkDevice::getMacAddress() const
{
    return m_macAddress;
}
QString CurrentNetworkDevice::getNetmask() const
{
    return m_netmask;
}
QString CurrentNetworkDevice::getBroadcastAddress() const
{
    return m_broadcastAddress;
}
void CurrentNetworkDevice::setInterfaceName(const QString& name)
{
    m_interfaceName = name;
}
void CurrentNetworkDevice::setIpAddress(const QString& ipAddress)
{
    m_ipAddress = ipAddress;
}
void CurrentNetworkDevice::setMacAddress(const QString& macAddress)
{
    m_macAddress = macAddress;
}
void CurrentNetworkDevice::setNetmask(const QString& netmask)
{
    m_netmask = netmask;
}
void CurrentNetworkDevice::setBroadcastAddress(const QString& broadcastAddress)
{
    m_broadcastAddress = broadcastAddress;
}
