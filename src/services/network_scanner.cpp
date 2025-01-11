#include "network_scanner.h"

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