#include "scan_controller.h"

#include "views/components/info_bar.h"

ScanController::ScanController(DeviceController *deviceController, InfoBar *infoBar, QObject *parent)
    : QObject(parent)
    , m_deviceController(deviceController)
    , m_networkScanner(NetworkScanner::getInstance())
    , m_infoBar(infoBar)
{
    
    m_currentDevice = new CurrentNetworkDevice();

    connect(m_networkScanner, &NetworkScanner::currentDeviceUpdated,
            this, &ScanController::handleCurrentDeviceUpdated);    
    connect(m_networkScanner, &NetworkScanner::deviceFound,
            this, &ScanController::handleDeviceFound);

    m_networkScanner->scanCurrentDevice(m_currentDevice);    
}

ScanController::~ScanController()
{
}

void ScanController::startScan()
{
    emit scanStarted();
    m_deviceController->clearDevices();
    // dependents of NetworkScanner
}

void ScanController::stopScan()
{
    // dependents of NetworkScanner
}

void ScanController::pingDevice(const QHostAddress &ipAddress)
{
    // dependents of NetworkScanner
}

void ScanController::handleDeviceFound(const NetworkDevice &device)
{
    m_deviceController->addDevice(device);
}

void ScanController::handleScanError(const QString &error)
{
    emit scanError(error);
}

void ScanController::handleScanComplete()
{
    emit scanFinished();
}

void ScanController::handleCurrentDeviceUpdated()
{
    m_infoBar->setInterfaceLabel(m_currentDevice->getInterfaceName());
    m_infoBar->setIpLabel(m_currentDevice->getIpAddress());
    m_infoBar->setMacLabel(m_currentDevice->getMacAddress());
    m_infoBar->setNetmaskLabel(m_currentDevice->getNetmask());
    m_infoBar->setBroadcastLabel(m_currentDevice->getBroadcastAddress());
}