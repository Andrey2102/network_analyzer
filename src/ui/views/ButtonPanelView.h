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

signals:
    void scanStarted();
    void scanProgressUpdated(int progress);
    void scanCompleted();
    void scanFailed(const QString& error);

private slots:
    void startScanning();
    void handleScanProgress(int progress);
    void handleScanFinished();
    void handleScanError(const QString& error);

private:
    void setupConnections();

    QPushButton* scanButton;
    QPushButton* pingButton;
    QPushButton* exportCsvButton;
    QPushButton* exportJsonButton;
    QVBoxLayout* buttonsLayout;
};

#endif // BUTTONPANELVIEW_H