#include "main_window.h"
#include <QApplication>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
    createMenuBar();
    createToolBar();
    createStatusBar();
}

MainWindow::~MainWindow()
{
}

void MainWindow::createMenuBar()
{
    // File Menu
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(tr("&Exit"), this, &QWidget::close);

    // Scan Menu
    scanMenu = menuBar()->addMenu(tr("&Scan"));
    scanMenu->addAction(tr("&Start Scan"), this, [this]() {
        statusBar()->showMessage(tr("Scanning network..."), 2000);
    });
    scanMenu->addAction(tr("&Stop Scan"));

    // Help Menu
    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(tr("&About"), this, [this]() {
        QMessageBox::about(this, tr("About Network Analyzer"),
            tr("Network Analyzer\n\n"
               "A tool for analyzing local network devices."));
    });
}

void MainWindow::createToolBar()
{
    mainToolBar = addToolBar(tr("Main ToolBar"));
    mainToolBar->addAction(tr("Scan"), this, [this]() {
        statusBar()->showMessage(tr("Starting network scan..."), 2000);
    });
    mainToolBar->addAction(tr("Stop"));
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::setupUI()
{
    setWindowTitle(tr("Network Analyzer"));
    resize(800, 600);
}