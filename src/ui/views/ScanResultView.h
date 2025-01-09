#ifndef SCANRESULTVIEW_H
#define SCANRESULTVIEW_H

#include "AView.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

class ScanResultView : public AView {
    Q_OBJECT

public:
    explicit ScanResultView(QWidget *parent = nullptr);
    virtual ~ScanResultView() {}

    void setupUI() override;

private:
    QLabel* resultLabel;
    QLabel* operationResult;
    QVBoxLayout* mainLayout;
};

#endif // SCANRESULTVIEW_H