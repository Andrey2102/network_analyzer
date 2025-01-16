#ifndef SCAN_RESULT_H
#define SCAN_RESULT_H

#include "../abstract/abstract_view.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableView>

class ScanResult : public AbstractView {
    Q_OBJECT

public:
    explicit ScanResult(QWidget *parent = nullptr);
    virtual ~ScanResult() {}

    void setupUI() override;

    // Геттеры
    QTableView* getResultTable() const { return resultTable; }
    QLabel* getStatusLabel() const { return resultLabel; }

private:
    QLabel* resultLabel;
    QTableView* resultTable;
    QVBoxLayout* mainLayout;
};

#endif // SCAN_RESULT_H