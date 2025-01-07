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
   : QMainWindow(parent)
{
   setupUI();
   createCentralWidget();
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

void MainWindow::createCentralWidget()
{
   centralWidget = new QWidget(this);
   setCentralWidget(centralWidget);
   
   // Main layout
   mainLayout = new QVBoxLayout(centralWidget);
   
   // Info section
   infoLayout = new QGridLayout();
   ipLabel = new QLabel("IP: N/A", this);
   macLabel = new QLabel("MAC: N/A", this);
   otherLabel1 = new QLabel("Other", this);
   otherLabel2 = new QLabel("Other", this);
   
   // Добавляем рамки вокруг лейблов
   QString labelStyle = "QLabel { border: 1px solid black; padding: 5px; }";
   ipLabel->setStyleSheet(labelStyle);
   macLabel->setStyleSheet(labelStyle);
   otherLabel1->setStyleSheet(labelStyle);
   otherLabel2->setStyleSheet(labelStyle);
   
   infoLayout->addWidget(ipLabel, 0, 0);
   infoLayout->addWidget(macLabel, 0, 1);
   infoLayout->addWidget(otherLabel1, 0, 2);
   infoLayout->addWidget(otherLabel2, 0, 3);
   
   // Operation result section
   QLabel *resultLabel = new QLabel("OPERATION RESULT", this);
   resultLabel->setAlignment(Qt::AlignCenter);
   
   operationResult = new QTextEdit(this);
   operationResult->setReadOnly(true);
   
   // Buttons
   QVBoxLayout *buttonsLayout = new QVBoxLayout();
   scanButton = new QPushButton("Scan network", this);
   pingButton = new QPushButton("Ping", this);
   exportCsvButton = new QPushButton("Export results (CSV)", this);
   exportJsonButton = new QPushButton("Export results (JSON)", this);
   
   // Устанавливаем фиксированную ширину для кнопок
   int buttonWidth = 200;
   scanButton->setFixedWidth(buttonWidth);
   pingButton->setFixedWidth(buttonWidth);
   exportCsvButton->setFixedWidth(buttonWidth);
   exportJsonButton->setFixedWidth(buttonWidth);
   
   buttonsLayout->addWidget(scanButton);
   buttonsLayout->addWidget(pingButton);
   buttonsLayout->addWidget(exportCsvButton);
   buttonsLayout->addWidget(exportJsonButton);
   buttonsLayout->addStretch();
   
   // Create a horizontal layout for the operation result and buttons
   QHBoxLayout *bottomLayout = new QHBoxLayout();
   QVBoxLayout *resultLayout = new QVBoxLayout();
   resultLayout->addWidget(resultLabel);
   resultLayout->addWidget(operationResult);
   
   bottomLayout->addLayout(resultLayout, 7); // Set stretch factor to 7
   bottomLayout->addLayout(buttonsLayout, 3); // Set stretch factor to 3
   
   // Layout assembly
   mainLayout->addLayout(infoLayout);
   mainLayout->addSpacing(10); // Add some spacing
   mainLayout->addLayout(bottomLayout);
}