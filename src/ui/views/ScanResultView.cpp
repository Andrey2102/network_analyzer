#include "ScanResultView.h"
#include "core/network_scanner.h"
#include <QTableView>
#include <QHeaderView>

ScanResultView::ScanResultView(QWidget *parent)
    : AView(parent),
      resultLabel(new QLabel(this)),
      deviceTable(new QTableView(this)),
      deviceModel(new DeviceListModel(this)),
      mainLayout(new QVBoxLayout(this))
{
    setupUI();
    setupConnections();
}

void ScanResultView::setupUI() {
    // Setup result label
    resultLabel->setText("Ready to scan");
    
    // Setup table
    deviceTable->setModel(deviceModel);
    deviceTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    deviceTable->setSelectionMode(QAbstractItemView::SingleSelection);
    deviceTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    
    // Setup layout
    mainLayout->addWidget(resultLabel);
    mainLayout->addWidget(deviceTable);
    setLayout(mainLayout);
}

void ScanResultView::setupConnections() {
    auto scanner = NetworkScanner::getInstance();
    connect(scanner, &NetworkScanner::deviceFound, this, &ScanResultView::handleNewDevice);
}

void ScanResultView::handleNewDevice(const NetworkDevice& device) {
    deviceModel->addDevice(device);
    updateStatusLabel();
}

void ScanResultView::updateStatusLabel() {
    int deviceCount = deviceModel->rowCount();
    resultLabel->setText(QString("Found %1 device%2")
        .arg(deviceCount)
        .arg(deviceCount != 1 ? "s" : ""));
}

void ScanResultView::handleScanStarted() {
    deviceModel->clear();
    resultLabel->setText("Scanning network...");
}

void ScanResultView::handleScanProgress(int progress) {
    resultLabel->setText(QString("Scanning network... %1%").arg(progress));
}

void ScanResultView::handleScanCompleted() {
    updateStatusLabel();
}

void ScanResultView::handleScanFailed(const QString& error) {
    resultLabel->setText(QString("Scan failed: %1").arg(error));
}