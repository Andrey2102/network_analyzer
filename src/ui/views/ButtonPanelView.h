#ifndef BUTTONPANELVIEW_H
#define BUTTONPANELVIEW_H

#include "AView.h"
#include <QPushButton>
#include <QVBoxLayout>

class ButtonPanelView : public AView {
    Q_OBJECT

public:
    explicit ButtonPanelView(QWidget *parent = nullptr);
    virtual ~ButtonPanelView() {}

    void setupUI() override;

private:
    QPushButton* scanButton;
    QPushButton* pingButton;
    QPushButton* exportCsvButton;
    QPushButton* exportJsonButton;
    QVBoxLayout* buttonsLayout;
};

#endif // BUTTONPANELVIEW_H