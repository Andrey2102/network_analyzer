#ifndef SCANRESULTVIEW_H
#define SCANRESULTVIEW_H

#include "AView.h"
#include "core/models/device_list_model.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QTableView>
#include "core/network_device.h"

class ScanResultView : public AView {
    Q_OBJECT

public:
    explicit ScanResultView(QWidget *parent = nullptr);
    virtual ~ScanResultView() {}

    void setupUI() override;

public slots:
    void handleScanStarted();
    void handleScanProgress(int progress);
    void handleScanCompleted();
    void handleScanFailed(const QString& error);
    void handleNewDevice(const NetworkDevice& device);

private:
    void setupConnections();
    void updateStatusLabel();

    QLabel* resultLabel;
    QTableView* deviceTable;
    DeviceListModel* deviceModel;
    QVBoxLayout* mainLayout;
};

#endif // SCANRESULTVIEW_H