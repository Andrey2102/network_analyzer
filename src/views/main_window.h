#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>
#include "controllers/device_controller.h"
#include "controllers/scan_controller.h"
#include "views/components/scan_result.h"
#include "views/components/button_panel.h"
#include "views/components/info_bar.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Handle scan operations
    void onScanStarted();
    void onScanFinished();
    void onScanError(const QString &error);
    
    // Handle device operations
    void onDeviceFound(const NetworkDevice &device);
    void onDeviceUpdated(const NetworkDevice &device);
    
    // Handle export operations
    void onExportFinished(bool success, const QString &message);

private:
    // UI setup methods
    void setupUI();
    void createMenus();
    void createToolbar();
    void createStatusBar();
    void setupConnections();

    // UI Components
    ScanResult* scanResult;
    ButtonPanel* buttonPanel;
    InfoBar* infoBar;

    // Controllers
    DeviceController* deviceController;
    ScanController* scanController;

    // UI helper methods
    void showExportDialog(bool isJson);
    void updateInfoBar(const NetworkDevice &device);
};

#endif // MAIN_WINDOW_H