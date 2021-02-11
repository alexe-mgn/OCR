#ifndef HOMETAB_H
#define HOMETAB_H

#include "ui_HomeTab.h"


class HomeTab : public QWidget, public Ui::HomeTab {
Q_OBJECT

public:
    explicit HomeTab(QWidget *parent = nullptr);
};


#endif
