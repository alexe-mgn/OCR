#ifndef INFOPANEL_H
#define INFOPANEL_H

#include <QtWidgets/QWidget>

#include "ui_InfoPanel.h"


class InfoPanel : public QWidget, public Ui::InfoPanel {
Q_OBJECT

public:
    explicit InfoPanel(QWidget *parent = nullptr);

    virtual void refresh() {};
};


#endif //INFOPANEL_H
