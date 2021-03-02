#ifndef TAB_H
#define TAB_H

#include <QtCore/QList>

#include <QtWidgets/QWidget>


class Tab : public QWidget {
public:
    explicit Tab(QWidget *parent = nullptr) : QWidget(parent) {}

    virtual QList<QWidget *> getPanels() { return QList<QWidget *>(); }

    virtual bool isSaveAvailable() { return false; }

    virtual bool isClearAvailable() { return false; };

public slots:

    virtual void clear() {};
};


#endif
