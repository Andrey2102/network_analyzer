#include "main_window.h"
#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , scanResult(new ScanResult(this))
    , buttonPanel(new ButtonPanel(this))
    , infoBar(new InfoBar(this))
    , deviceController(new DeviceController(this))
    , scanController(new ScanController(deviceController, infoBar, this))
{
    setupUI();
    createMenus();
    createToolbar();
    createStatusBar();
    setupConnections();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    // Create main layout
    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->addWidget(infoBar);

    // Create horizontal layout for buttons and results
    QHBoxLayout* contentLayout = new QHBoxLayout();
    contentLayout->addWidget(buttonPanel);
    contentLayout->addWidget(scanResult, 1); // Give scan result more space

    mainLayout->addLayout(contentLayout);

    // Set up central widget
    QWidget* centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    // Configure window
    resize(800, 600);
    setWindowTitle("Network Analyzer");

    // Set up the model for the result view
    scanResult->getResultTable()->setModel(deviceController->getModel());
}

void MainWindow::setupConnections()
{
    // Connect button signals to controller slots
    connect(buttonPanel->getScanButton(), &QPushButton::clicked,
            scanController, &ScanController::startScan);

    // Export buttons
    connect(buttonPanel->getExportCsvButton(), &QPushButton::clicked,
            [this]() { showExportDialog(false); });

    connect(buttonPanel->getExportJsonButton(), &QPushButton::clicked,
            [this]() { showExportDialog(true); });

    // Connect scan controller signals
    connect(scanController, &ScanController::scanStarted,
            this, &MainWindow::onScanStarted);
    connect(scanController, &ScanController::scanFinished,
            this, &MainWindow::onScanFinished);
    connect(scanController, &ScanController::scanError,
            this, &MainWindow::onScanError);

    // Connect device controller signals
    connect(deviceController, &DeviceController::deviceAdded,
            this, &MainWindow::onDeviceFound);
    connect(deviceController, &DeviceController::deviceUpdated,
            this, &MainWindow::onDeviceUpdated);
    connect(deviceController, &DeviceController::exportFinished,
            this, &MainWindow::onExportFinished);

    // Connect table selection to info bar update
    connect(scanResult->getResultTable()->selectionModel(), 
            &QItemSelectionModel::currentChanged,
            [this](const QModelIndex &current, const QModelIndex&) {
                if (current.isValid()) {
                    NetworkDevice device = deviceController->getModel()->getDeviceAt(current);
                    updateInfoBar(device);
                }
            });
}

void MainWindow::onScanStarted()
{
    buttonPanel->getScanButton()->setEnabled(false);
    statusBar()->showMessage("Scanning network...");
}

void MainWindow::onScanFinished()
{
    buttonPanel->getScanButton()->setEnabled(true);
    statusBar()->showMessage("Scan completed", 3000);
}

void MainWindow::onScanError(const QString &error)
{
    buttonPanel->getScanButton()->setEnabled(true);
    statusBar()->showMessage("Error: " + error, 5000);
    QMessageBox::warning(this, "Scan Error", error);
}

void MainWindow::onDeviceFound(const NetworkDevice &device)
{
    statusBar()->showMessage("Found device: " + device.getIpAddress().toString(), 2000);
}

void MainWindow::onDeviceUpdated(const NetworkDevice &device)
{
    statusBar()->showMessage("Updated device: " + device.getIpAddress().toString(), 2000);
}

void MainWindow::onExportFinished(bool success, const QString &message)
{
    if (success) {
        statusBar()->showMessage(message, 3000);
    } else {
        statusBar()->showMessage("Export error: " + message, 5000);
        QMessageBox::warning(this, "Export Error", message);
    }
}

void MainWindow::showExportDialog(bool isJson)
{
    QString filter = isJson ? "JSON files (*.json)" : "CSV files (*.csv)";
    QString suffix = isJson ? ".json" : ".csv";
    
    QString filePath = QFileDialog::getSaveFileName(this,
        "Export Devices", 
        "network_devices" + suffix,
        filter);
    
    if (!filePath.isEmpty()) {
        if (isJson) {
            deviceController->exportToJson(filePath);
        } else {
            deviceController->exportToCsv(filePath);
        }
    }
}

void MainWindow::updateInfoBar(const NetworkDevice &device)
{
    // infoBar->getIpLabel()->setText("IP: " + device.getIpAddress().toString());
    // infoBar->getMacLabel()->setText("MAC: " + device.getMacAddress());
    // infoBar->getOtherInfoLabel()->setText("Status: " + device.getStatusString());
}

void MainWindow::createMenus()
{
    // Menu implementation...
}

void MainWindow::createToolbar()
{
    // Toolbar implementation...
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage("Ready");
}