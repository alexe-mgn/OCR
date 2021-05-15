#ifndef HOMETAB_H
#define HOMETAB_H

#include <QtCore/QList>

#include <QtWidgets/QWidget>

#include "ui_HomeTab.h"
#include "Tabs/Tab.h"
#include "MainWindow.h"


class HomeTab : public Tab, public Ui::HomeTab {
Q_OBJECT

public:
    explicit HomeTab(MainWindow *mainWindow = nullptr, QWidget *parent = nullptr) : Tab(mainWindow, parent) {
        setupUi(this);
    };

    ~HomeTab() override {};
};


#endif
