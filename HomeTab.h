#ifndef HOMETAB_H
#define HOMETAB_H

#include "ui_HomeTab.h"
#include "Tab.h"


class HomeTab : public Tab, public Ui::HomeTab {
Q_OBJECT

public:
    explicit HomeTab(QWidget *parent = nullptr);

    bool isSaveAvailable() override;
};


#endif
