#include "scan_controller.h"

ScanController::ScanController(DeviceController *deviceController, QObject *parent)
    : QObject(parent)
    , m_deviceController(deviceController)
    , m_networkScanner(NetworkScanner::getInstance())
{
    // Подключаем сигналы сканера
    connect(m_networkScanner, &NetworkScanner::deviceFound,
            this, &ScanController::handleDeviceFound);
    // Тут будут другие подключения сигналов от NetworkScanner,
    // когда они будут реализованы в сервисе
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