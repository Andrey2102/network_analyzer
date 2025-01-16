#ifndef INFO_BAR_H
#define INFO_BAR_H

#include "../abstract/abstract_view.h"
#include <QLabel>
#include <QHBoxLayout>

class InfoBar : public AbstractView {
    Q_OBJECT

public:
    explicit InfoBar(QWidget *parent = nullptr);
    virtual ~InfoBar() {}

    void setupUI() override;

    QLabel* getInterfaceLabel() const { return interfaceLabel; }
    QLabel* getIpLabel() const { return ipLabel; }
    QLabel* getMacLabel() const { return macLabel; }
    QLabel* getNetmaskLabel() const { return netLabel; }
    QLabel* getBroadcastLabel() const { return broadcastLabel; }

    void setInterfaceLabel(const QString &text) { interfaceLabel->setText( "Interface Name: " + text); }
    void setIpLabel(const QString &text) { ipLabel->setText( "IP: " + text); }
    void setMacLabel(const QString &text) { macLabel->setText( "MAC: " + text); }
    void setNetmaskLabel(const QString &text) { netLabel->setText( "Netmask: " + text); }
    void setBroadcastLabel(const QString &text) { broadcastLabel->setText( "Broadcast Address: " + text); }

private:
    QLabel* interfaceLabel;
    QLabel* ipLabel;
    QLabel* macLabel;
    QLabel* netLabel;
    QLabel* broadcastLabel;
    QHBoxLayout* infoLayout;
};

#endif // INFO_BAR_H