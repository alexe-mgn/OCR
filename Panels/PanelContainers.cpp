#include "PanelContainers.h"


PanelContainer::PanelContainer(QWidget *parent) : QStackedWidget(parent) {
    setContentsMargins(0, 0, 0, 0);
}

void PanelContainer::connectTab(Tab *tab, QWidget *widget) {
    tabWidgets_.insert(tab, widget);
    addWidget(widget);
    currentTab_ = tabWidgets_.key(currentWidget());
}

QList<Tab *> PanelContainer::tabs() {
    return tabWidgets_.keys();
}

Tab *PanelContainer::currentTab() {
    return currentTab_;
}

QMap<Tab *, QWidget *> PanelContainer::tabWidgets() {
    return tabWidgets_;
}

bool PanelContainer::setCurrentTab(Tab *newTab) {
    auto widget = tabWidgets_.value(newTab, nullptr);
    if (widget) {
        currentTab_ = newTab;
        setCurrentWidget(widget);
        setWindowTitle(widget->windowTitle());
        return true;
    } else
        return false;
}

void PanelContainer::removeTab(Tab *tab) {
    removeWidget(tabWidgets_[tab]);
    tabWidgets_.remove(tab);
//    tab->setParent(nullptr);
    if (!tabWidgets_.empty())
        currentTab_ = tabWidgets_.key(currentWidget());
    else
        currentTab_ = nullptr;
}


////////////////////


PanelDockWidget::PanelDockWidget(QWidget *parent,
                                 PanelContainer *container) : QDockWidget(parent) {
    if (!container)
        container_ = new PanelContainer();
    setContentsMargins(0, 0, 0, 0);
}

PanelContainer *PanelDockWidget::container() {
    return container_;
}

void PanelDockWidget::setContainer(PanelContainer *newContainer) {
    if (container_) {
        auto tws = container_->tabWidgets();
        for (auto tw = tws.begin(); tw != tws.end(); ++tw) {
            container_->removeTab(tw.key());
            newContainer->connectTab(tw.key(), tw.value());
        }
    }
    container_ = newContainer;
}

void PanelDockWidget::connectTab(Tab *tab, QWidget *widget) {
    container_->connectTab(tab, widget);
}

bool PanelDockWidget::setCurrentTab(Tab *newTab) {
    bool haveTab = container_->setCurrentTab(newTab);
    if (haveTab) {
        show();
        setWindowTitle(container_->windowTitle());
        return true;
    } else {
        hide();
        return false;
    }
}

void PanelDockWidget::removeTab(Tab *tab) {
    container_->removeTab(tab);
}