#include "ButtonPanelView.h"
#include "core/network_scanner.h"

ButtonPanelView::ButtonPanelView(QWidget *parent)
    : AView(parent),
      scanButton(new QPushButton("Scan", this)),
      pingButton(new QPushButton("Ping", this)),
      exportCsvButton(new QPushButton("Export CSV", this)),
      exportJsonButton(new QPushButton("Export JSON", this)),
      buttonsLayout(new QVBoxLayout(this))
{
    setupUI();
    setupConnections();
}

void ButtonPanelView::setupUI() {
    buttonsLayout->addWidget(scanButton);
    buttonsLayout->addWidget(pingButton);
    buttonsLayout->addWidget(exportCsvButton);
    buttonsLayout->addWidget(exportJsonButton);
    buttonsLayout->addStretch();
    setLayout(buttonsLayout);
}

void ButtonPanelView::setupConnections() {
    // Connect scan button to scanner
    connect(scanButton, &QPushButton::clicked, this, &ButtonPanelView::startScanning);
    
    // Connect scanner signals
    auto scanner = NetworkScanner::getInstance();
    connect(scanner, &NetworkScanner::scanProgress, this, &ButtonPanelView::handleScanProgress);
    connect(scanner, &NetworkScanner::scanFinished, this, &ButtonPanelView::handleScanFinished);
    connect(scanner, &NetworkScanner::scanError, this, &ButtonPanelView::handleScanError);
}

void ButtonPanelView::startScanning() {
    scanButton->setEnabled(false);
    scanButton->setText("Scanning...");
    emit scanStarted();
    
    auto scanner = NetworkScanner::getInstance();
    // TODO: Make IP range configurable through settings
    scanner->startScan("192.168.1.1-192.168.1.255");
}

void ButtonPanelView::handleScanProgress(int progress) {
    scanButton->setText(QString("Scanning... %1%").arg(progress));
    emit scanProgressUpdated(progress);
}

void ButtonPanelView::handleScanFinished() {
    scanButton->setEnabled(true);
    scanButton->setText("Scan");
    emit scanCompleted();
}

void ButtonPanelView::handleScanError(const QString& error) {
    scanButton->setEnabled(true);
    scanButton->setText("Scan");
    emit scanFailed(error);
}