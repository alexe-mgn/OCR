#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QList>
#include <QtCore/QDir>

#include <QtWidgets/QWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTabBar>

#include "PanelContainers.h"
#include "Tab.h"
#include "HomeTab.h"
#include "ui_MainWindow.h"


class MainWindow : public QMainWindow, public Ui::MainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    QTabBar *tabBar;
    QStackedWidget *stack;
    HomeTab *homeTab;
    QList<PanelDockWidget *> panelDockWidgets;

    QDir fileLastPath;

    void connectTab(Tab *tab);

    void addTab(Tab *tab);

//    bool loadFile(const QString &path);

    void chooseFile();

    void closeTab(int index);

    void setCurrentIndex(int index);

    void moveTab(int from, int to);

protected:
    void uiTabMoved(int from, int to);
};

#endif