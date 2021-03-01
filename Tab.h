#ifndef TAB_H
#define TAB_H

#include <QtCore/QList>

#include <QtWidgets/QWidget>


class Tab : public virtual QWidget {
public:
    explicit Tab(QWidget *parent = nullptr) : QWidget(parent) {}

    virtual QList<QWidget *> getPanels() { return QList<QWidget *>(); }

    virtual bool isSaveAvailable() { return false; }

    virtual void clear() {};

    virtual bool isClearAvailable() { return false; };
};


#endif
