#ifndef HOMETAB_H
#define HOMETAB_H

#include <QtCore/QList>

#include <QtWidgets/QWidget>

#include "Tab.h"
#include "ui_HomeTab.h"


class HomeTab : public Tab, public Ui::HomeTab {
Q_OBJECT

public:
    explicit HomeTab(QWidget *parent = nullptr);

    QList<QWidget *> getPanels() override;

    bool isSaveAvailable() override;
};


#endif
