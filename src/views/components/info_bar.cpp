#include "info_bar.h"

InfoBar::InfoBar(QWidget *parent)
    : AbstractView(parent),
      ipLabel(new QLabel("IP: ", this)),
      macLabel(new QLabel("MAC: ", this)),
      otherInfoLabel(new QLabel("Other Info: ", this)),
      infoLayout(new QHBoxLayout(this))
{
    setupUI();
    setStyleSheet("border: 1px solid black;");
    infoLayout->setContentsMargins(5, 5, 5, 5);
}

void InfoBar::setupUI() {
    infoLayout->addWidget(ipLabel);
    infoLayout->addWidget(macLabel);
    infoLayout->addWidget(otherInfoLabel);
    setLayout(infoLayout);
}