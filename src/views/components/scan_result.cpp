#include "scan_result.h"

ScanResult::ScanResult(QWidget *parent)
    : AbstractView(parent),
      resultLabel(new QLabel("Scan Results", this)),
      resultTable(new QTableView(this)),
      mainLayout(new QVBoxLayout(this))
{
    setupUI();
    setStyleSheet("border: 1px solid black;");
}

void ScanResult::setupUI() {
    resultLabel->setAlignment(Qt::AlignCenter);
    
    mainLayout->addWidget(resultLabel);
    mainLayout->addWidget(resultTable);
    setLayout(mainLayout);
}