#ifndef CURRENT_NETWORK_DEVICE_H
#define CURRENT_NETWORK_DEVICE_H

#include <QString>

class CurrentNetworkDevice {
public:
    // Constructor
    CurrentNetworkDevice();
    CurrentNetworkDevice(const QString& interfaceName, const QString& ipAddress,
                         const QString& macAddress, const QString& netmask,
                         const QString& broadcastAddress);
    
    // Copy constructor
    CurrentNetworkDevice(const CurrentNetworkDevice& other);
    CurrentNetworkDevice& operator=(const CurrentNetworkDevice& other);
    
    // Move constructor
    CurrentNetworkDevice(CurrentNetworkDevice&& other) noexcept;
    CurrentNetworkDevice& operator=(CurrentNetworkDevice&& other) noexcept;
    
    // Destructor
    ~CurrentNetworkDevice();
    
    // Getters
    QString getInterfaceName() const;
    QString getIpAddress() const;
    QString getMacAddress() const;
    QString getNetmask() const;
    QString getBroadcastAddress() const;

    // Setters
    void setInterfaceName(const QString& name);
    void setIpAddress(const QString& ipAddress);
    void setMacAddress(const QString& macAddress);
    void setNetmask(const QString& netmask);
    void setBroadcastAddress(const QString& broadcastAddress);

private:
    QString m_interfaceName;
    QString m_ipAddress;
    QString m_macAddress;
    QString m_netmask;
    QString m_broadcastAddress;
};

#endif // CURRENT_NETWORK_DEVICE_H