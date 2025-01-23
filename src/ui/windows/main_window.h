#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include "ui/views/ScanResultView.h"
#include "ui/views/ButtonPanelView.h"
#include "ui/views/InfoBarView.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void createMenuBar();
    void createToolBar();
    void createStatusBar();
    void setupUI();
    void setupConnections();
    void createCentralWidget();

private:
    ScanResultView* scanResultView;
    ButtonPanelView* buttonPanelView;
    InfoBarView* infoBarView;
};

#endif // MAIN_WINDOW_H