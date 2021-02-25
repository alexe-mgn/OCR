#ifndef DOCKPANELS_H
#define DOCKPANELS_H

#include <QtCore/QList>
#include <QtCore/QMap>

#include <QtWidgets/QWidget>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QDockWidget>

#include "Tab.h"


class PanelContainer : public QStackedWidget {
Q_OBJECT

public:
    explicit PanelContainer(QWidget *parent = nullptr);

    void connectTab(Tab *tab, QWidget *widget);

    QList<Tab *> tabs();

    QMap<Tab *, QWidget *> tabWidgets();

    Tab *currentTab();

    bool setCurrentTab(Tab *newTab);

    void removeTab(Tab *tab);

protected:
    QMap<Tab *, QWidget *> tabWidgets_;
    Tab *currentTab_ = nullptr;
};


////////////////////


class PanelDockWidget : public QDockWidget {
Q_OBJECT

public:
    explicit PanelDockWidget(QWidget *parent = nullptr,
                             PanelContainer *container = nullptr);


    void setContainer(PanelContainer *newContainer);

    PanelContainer *container();

    void connectTab(Tab *tab, QWidget *widget);

    bool setCurrentTab(Tab *newTab);

    void removeTab(Tab *tab);

protected:
    PanelContainer *container_ = nullptr;
};

#endif //DOCKPANELS_H
