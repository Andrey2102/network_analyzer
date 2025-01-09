#include "ScanResultView.h"

ScanResultView::ScanResultView(QWidget *parent)
    : AView(parent),
      resultLabel(new QLabel(this)),
      operationResult(new QLabel(this)),
      mainLayout(new QVBoxLayout(this))
{
    setupUI();
    setStyleSheet("border: 1px solid black;"); // Add border to ScanResultView
}

void ScanResultView::setupUI() {

    QVBoxLayout *resultLayout = new QVBoxLayout();
    resultLayout->addWidget(resultLabel);
    resultLayout->addWidget(operationResult);
    setLayout(mainLayout);
}