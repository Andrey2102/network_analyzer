#ifndef BUTTON_PANEL_H
#define BUTTON_PANEL_H

#include "../abstract/abstract_view.h"
#include <QPushButton>
#include <QVBoxLayout>

class ButtonPanel : public AbstractView {
    Q_OBJECT

public:
    explicit ButtonPanel(QWidget *parent = nullptr);
    virtual ~ButtonPanel() {}

    void setupUI() override;
    
    // Геттеры для доступа к кнопкам
    QPushButton* getScanButton() const { return scanButton; }
    QPushButton* getPingButton() const { return pingButton; }
    QPushButton* getExportCsvButton() const { return exportCsvButton; }
    QPushButton* getExportJsonButton() const { return exportJsonButton; }

private:
    QPushButton* scanButton;
    QPushButton* pingButton;
    QPushButton* exportCsvButton;
    QPushButton* exportJsonButton;
    QVBoxLayout* buttonsLayout;
};

#endif // BUTTON_PANEL_H