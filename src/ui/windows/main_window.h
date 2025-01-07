#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>

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

private:
    QMenu *fileMenu;
    QMenu *scanMenu;
    QMenu *helpMenu;
    QToolBar *mainToolBar;
};

#endif // MAINWINDOW_H