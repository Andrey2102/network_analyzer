#ifndef NETWORK_DEVICE_H
#define NETWORK_DEVICE_H

#include <QString>
#include <QHostAddress>
#include <QDateTime>

class NetworkDevice
{
public:
    // Device status enum
    enum class Status {
        Unknown,
        Online,
        Offline,
        Unreachable
    };

    // Constructors
    NetworkDevice();
    NetworkDevice(const QHostAddress& ip);
    NetworkDevice(const QHostAddress& ip, const QString& mac);

    // Getters
    QHostAddress getIpAddress() const;
    QString getMacAddress() const;
    QString getHostname() const;
    Status getStatus() const;
    QDateTime getLastSeen() const;
    int getResponseTime() const;

    // Setters
    void setIpAddress(const QHostAddress& ip);
    void setMacAddress(const QString& mac);
    void setHostname(const QString& hostname);
    void setStatus(Status status);
    void setLastSeen(const QDateTime& timestamp);
    void setResponseTime(int milliseconds);

    // Utility methods
    bool isOnline() const;
    QString getStatusString() const;
    bool operator==(const NetworkDevice& other) const;
    
private:
    QHostAddress m_ipAddress;
    QString m_macAddress;
    QString m_hostname;
    Status m_status;
    QDateTime m_lastSeen;
    int m_responseTime;  // in milliseconds
};

#endif // NETWORK_DEVICE_H