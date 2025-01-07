#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void createMenuBar();
    void createToolBar();
    void createStatusBar();
    void setupUI();
    void createCentralWidget();

private:
    // Menus
    QMenu *fileMenu;
    QMenu *scanMenu;
    QMenu *helpMenu;
    QToolBar *mainToolBar;
    
    // Central widget components
    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    QGridLayout *infoLayout;
    
    // Info labels
    QLabel *ipLabel;
    QLabel *macLabel;
    QLabel *otherLabel1;
    QLabel *otherLabel2;
    
    // Operation result
    QTextEdit *operationResult;
    
    // Buttons
    QPushButton *scanButton;
    QPushButton *pingButton;
    QPushButton *exportCsvButton;
    QPushButton *exportJsonButton;
};

#endif // MAINWINDOW_H