#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QList>
#include <QtCore/QDir>

#include <QtWidgets/QWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTabBar>

#include "ui_MainWindow.h"
#include "Panels/PanelContainers.h"
#include "Tabs/Tab.h"
#include "Tabs/HomeTab.h"


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

    void updateActions();

//    bool loadFile(const QString &path);

    void openImage(const QImage &image);

    void chooseOpenFile();

    void openCamera();

    void closeTab(int index);

    [[nodiscard]] Tab *currentTab() const;

    void setCurrentIndex(int index);

    void moveTab(int from, int to);

protected:
    void uiTabMoved(int from, int to);
};

#endif