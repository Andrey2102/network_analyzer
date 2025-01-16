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

    QLabel* getIpLabel() const { return ipLabel; }
    QLabel* getMacLabel() const { return macLabel; }
    QLabel* getOtherInfoLabel() const { return otherInfoLabel; }

private:
    QLabel* ipLabel;
    QLabel* macLabel;
    QLabel* otherInfoLabel;
    QHBoxLayout* infoLayout;
};

#endif // INFO_BAR_H