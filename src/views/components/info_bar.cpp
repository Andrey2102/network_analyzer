#include "info_bar.h"

InfoBar::InfoBar(QWidget *parent)
    : AbstractView(parent),
      interfaceLabel(new QLabel("Interface Name: N/A", this)),
      ipLabel(new QLabel("IP: N/A ", this)),
      macLabel(new QLabel("MAC: N/A ", this)),
      netLabel(new QLabel("Netmask: N/A ", this)),
      broadcastLabel(new QLabel("Broadcast Address: N/A ", this)),
      infoLayout(new QHBoxLayout(this))
{
    setupUI();
    setStyleSheet("border: 1px solid black;");
    infoLayout->setContentsMargins(5, 5, 5, 5);
}

void InfoBar::setupUI() {
    infoLayout->addWidget(interfaceLabel);
    infoLayout->addWidget(ipLabel);
    infoLayout->addWidget(macLabel);
    infoLayout->addWidget(netLabel);
    infoLayout->addWidget(broadcastLabel);
    setLayout(infoLayout);
}