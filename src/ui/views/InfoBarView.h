#ifndef INFOBARVIEW_H
#define INFOBARVIEW_H

#include "AView.h"
#include <QLabel>
#include <QHBoxLayout>

class InfoBarView : public AView {
    Q_OBJECT

public:
    explicit InfoBarView(QWidget *parent = nullptr);
    virtual ~InfoBarView() {}

    void setupUI() override;

private:
    QLabel* ipLabel;
    QLabel* macLabel;
    QLabel* otherInfoLabel;
    QHBoxLayout* infoLayout;
};

#endif // INFOBARVIEW_H