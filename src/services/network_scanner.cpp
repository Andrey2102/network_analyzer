#include "network_scanner.h"

#include <QNetworkInterface>
#include <QProcess>

NetworkScanner* NetworkScanner::m_instance = nullptr;

NetworkScanner::NetworkScanner(QObject *parent)
    : QObject(parent)
    , m_isScanning(false)
{
    scanCurrentDevice();
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

void NetworkScanner::scanCurrentDevice()
{
    QString activeInterfaceName = getActiveLocalInterfaceName();
    if (activeInterfaceName.isEmpty()) {
        qInfo() << "No active interface found!";
        return;
    }

    const QList<QNetworkInterface>& interfaces = QNetworkInterface::allInterfaces();
    for (const QNetworkInterface& iface : interfaces) {
        if (iface.name() == activeInterfaceName) {
            qInfo() << "Interface Name:" << iface.humanReadableName();
            qInfo() << "Hardware (MAC) Address:" << iface.hardwareAddress();

            const QList<QNetworkAddressEntry>& entries = iface.addressEntries();
            for (const QNetworkAddressEntry& entry : entries) {
                if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol) {
                    qInfo() << "IPv4 Address:" << entry.ip().toString();
                    qInfo() << "Netmask:" << entry.netmask().toString();
                    qInfo() << "Broadcast Address:" << entry.broadcast().toString();
                }
            }
            return; // Завершаем, так как нужный интерфейс найден.
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