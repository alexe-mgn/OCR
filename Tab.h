#ifndef TAB_H
#define TAB_H

#include <QtCore/QList>

#include <QtWidgets/QWidget>


class Tab : public virtual QWidget {
public:
    explicit Tab(QWidget *parent = nullptr) : QWidget(parent) {}

    virtual QList<QWidget *> getPanels() = 0;

    virtual bool isSaveAvailable() = 0;
};


#endif
