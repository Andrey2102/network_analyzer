#include "main_window.h"
#include <QApplication>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel> 
#include <QPushButton>
#include <QTextEdit>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      scanResultView(new ScanResultView(this)),
      buttonPanelView(new ButtonPanelView(this)),
      infoBarView(new InfoBarView(this))
{
    setupUI();
    createMenuBar();
    createToolBar();
    createStatusBar();
    setupConnections();
}

MainWindow::~MainWindow()
{
    // Destructor implementation
}


void MainWindow::setupConnections() {
    // Connect button panel signals to scan result view
    connect(buttonPanelView, &ButtonPanelView::scanStarted,
            scanResultView, &ScanResultView::handleScanStarted);
    
    connect(buttonPanelView, &ButtonPanelView::scanProgressUpdated,
            scanResultView, &ScanResultView::handleScanProgress);
    
    connect(buttonPanelView, &ButtonPanelView::scanCompleted,
            scanResultView, &ScanResultView::handleScanCompleted);
    
    connect(buttonPanelView, &ButtonPanelView::scanFailed,
            scanResultView, &ScanResultView::handleScanFailed);
}

void MainWindow::createMenuBar()
{
}

void MainWindow::createToolBar()
{
}

void MainWindow::createStatusBar()
{
}

void MainWindow::setupUI()
{
   QVBoxLayout* mainLayout = new QVBoxLayout();

   // Add the InfoBarView at the top
   mainLayout->addWidget(infoBarView);

   // Create a horizontal layout for the button panel and result view
   QHBoxLayout* contentLayout = new QHBoxLayout();
   contentLayout->addWidget(buttonPanelView);
   contentLayout->addWidget(scanResultView);

   // Add the content layout to the main layout
   mainLayout->addLayout(contentLayout);

   QWidget* centralWidget = new QWidget(this);
   centralWidget->setLayout(mainLayout);
   setCentralWidget(centralWidget);
}