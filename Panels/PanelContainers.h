#ifndef PANELCONTAINERS_H
#define PANELCONTAINERS_H

#include <QtCore/QList>
#include <QtCore/QMap>

#include <QtWidgets/QWidget>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QDockWidget>

#include "Tabs/Tab.h"


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

    PanelContainer *container();

    void setContainer(PanelContainer *newContainer);

    void connectTab(Tab *tab, QWidget *widget);

    bool setCurrentTab(Tab *newTab);

    void removeTab(Tab *tab);

protected:
    PanelContainer *container_ = nullptr;
};

#endif //PANELCONTAINERS_H
