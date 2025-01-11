#include "network_device.h"

NetworkDevice::NetworkDevice()
    : m_status(Status::Unknown)
    , m_responseTime(-1)
{
}

NetworkDevice::NetworkDevice(const QHostAddress& ip)
    : m_ipAddress(ip)
    , m_status(Status::Unknown)
    , m_responseTime(-1)
{
}

NetworkDevice::NetworkDevice(const QHostAddress& ip, const QString& mac)
    : m_ipAddress(ip)
    , m_macAddress(mac)
    , m_status(Status::Unknown)
    , m_responseTime(-1)
{
}

// Getters implementation
QHostAddress NetworkDevice::getIpAddress() const
{
    return m_ipAddress;
}

QString NetworkDevice::getMacAddress() const
{
    return m_macAddress;
}

QString NetworkDevice::getHostname() const
{
    return m_hostname;
}

NetworkDevice::Status NetworkDevice::getStatus() const
{
    return m_status;
}

QDateTime NetworkDevice::getLastSeen() const
{
    return m_lastSeen;
}

int NetworkDevice::getResponseTime() const
{
    return m_responseTime;
}

// Setters implementation
void NetworkDevice::setIpAddress(const QHostAddress& ip)
{
    m_ipAddress = ip;
}

void NetworkDevice::setMacAddress(const QString& mac)
{
    m_macAddress = mac;
}

void NetworkDevice::setHostname(const QString& hostname)
{
    m_hostname = hostname;
}

void NetworkDevice::setStatus(Status status)
{
    m_status = status;
    if (status == Status::Online) {
        m_lastSeen = QDateTime::currentDateTime();
    }
}

void NetworkDevice::setLastSeen(const QDateTime& timestamp)
{
    m_lastSeen = timestamp;
}

void NetworkDevice::setResponseTime(int milliseconds)
{
    m_responseTime = milliseconds;
}

// Utility methods implementation
bool NetworkDevice::isOnline() const
{
    return m_status == Status::Online;
}

QString NetworkDevice::getStatusString() const
{
    switch (m_status) {
        case Status::Online:
            return QStringLiteral("Online");
        case Status::Offline:
            return QStringLiteral("Offline");
        case Status::Unreachable:
            return QStringLiteral("Unreachable");
        default:
            return QStringLiteral("Unknown");
    }
}

bool NetworkDevice::operator==(const NetworkDevice& other) const
{
    return m_ipAddress == other.m_ipAddress &&
           m_macAddress == other.m_macAddress;
}