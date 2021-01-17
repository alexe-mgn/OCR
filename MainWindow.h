#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_MainWindow.h"
#include "ImageViewer.h"


class MainWindow : public QMainWindow, public Ui::MainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    QDir fileLastPath;

    void connectTab(ImageViewer *tab);

    void chooseFile();

private slots:

    void on_tabWidget_tabCloseRequested(int index);

    void on_actionOpen_triggered();

    void on_actionExit_triggered();
};

#endif