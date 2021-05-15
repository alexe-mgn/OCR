#ifndef TAB_H
#define TAB_H

#include <QtCore/QList>

#include <QtWidgets/QWidget>


class MainWindow;


class Tab : public QWidget {
public:
    explicit Tab(MainWindow *mainWindow = nullptr, QWidget *parent = nullptr) : QWidget(parent) {
        main = mainWindow;
    }

    virtual QList<QWidget *> getPanels() { return QList<QWidget *>(); }

    virtual bool isSaveAvailable() { return false; }

    virtual bool isExportAvailable() { return false; }

    virtual bool isClearAvailable() { return false; };

public slots:

    virtual void clear() {};

public:
    MainWindow *main;
};


#endif
