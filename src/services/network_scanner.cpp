#include "network_scanner.h"

#include <QNetworkInterface>
#include <QProcess>

NetworkScanner* NetworkScanner::m_instance = nullptr;

NetworkScanner::NetworkScanner(QObject *parent)
    : QObject(parent)
    , m_isScanning(false)
{
}

NetworkScanner::~NetworkScanner()
{
}

NetworkScanner* NetworkScanner::getInstance()
{
    if (m_instance == nullptr) {
        m_instance = new NetworkScanner();
    }
    return m_instance;
}

QString NetworkScanner::getActiveLocalInterfaceName() 
{
    QProcess process;
    process.start("sh", QStringList() << "-c" << "ip route show default | awk '/default/ {print $5}'");
    process.waitForFinished();

    QString output = process.readAllStandardOutput().trimmed();
    return output;
}

void NetworkScanner::scanCurrentDevice(CurrentNetworkDevice *device)
{
    QString activeInterfaceName = getActiveLocalInterfaceName();
    if (activeInterfaceName.isEmpty()) {
        qInfo() << "No active interface found!";
        return;
    }

    const QList<QNetworkInterface>& interfaces = QNetworkInterface::allInterfaces();
    for (const QNetworkInterface& iface : interfaces) {
        if (iface.name() == activeInterfaceName) {
            device->setInterfaceName(iface.humanReadableName());
            device->setMacAddress(iface.hardwareAddress());

            const QList<QNetworkAddressEntry>& entries = iface.addressEntries();
            for (const QNetworkAddressEntry& entry : entries) {
                if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol) {
                    device->setIpAddress(entry.ip().toString());
                    device->setNetmask(entry.netmask().toString());
                    device->setBroadcastAddress(entry.broadcast().toString());
                }
            }
            emit(currentDeviceUpdated());
            return; // Found the active interface
        }
    }

    qInfo() << "Active interface" << activeInterfaceName << "not found in system interfaces.";
}

void NetworkScanner::startScan()
{
    if (m_isScanning) {
        return;
    }
    
    m_isScanning = true;
    m_devices.clear();

    // TODO: Implement actual network scanning
    // For now, just emit some test data
    NetworkDevice testDevice(QHostAddress("192.168.1.1"));
    testDevice.setMacAddress("00:11:22:33:44:55");
    testDevice.setHostname("test-device");
    testDevice.setStatus(NetworkDevice::Status::Online);
    
    emit deviceFound(testDevice);
    emit scanComplete();
}

void NetworkScanner::stopScan()
{
    if (!m_isScanning) {
        return;
    }
    
    m_isScanning = false;
    emit scanComplete();
}

void NetworkScanner::pingDevice(const QHostAddress &address)
{
    // TODO: Implement actual ping
    // For now, just emit some test response
    NetworkDevice device(address);
    device.setResponseTime(10);  // 10ms response time
    emit deviceFound(device);
}