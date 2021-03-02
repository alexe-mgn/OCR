#ifndef DATAVIEWPANEL_H
#define DATAVIEWPANEL_H


#include <QtWidgets/QWidget>

#include "TextData.h"
#include "ui_DataViewPanel.h"


class DataViewPanel : public QWidget, public Ui::DataViewPanel {
Q_OBJECT

public:
    explicit DataViewPanel(QWidget *parent = nullptr);

    virtual TextItem *item();

    bool isCreationAvailable();

public slots:

    void setItem(TextItem *textItem);

    virtual void removeItem();

    virtual void setEditable(bool enabled);

    virtual void refresh();

    virtual void refreshRanges() {};

    virtual void refreshItemData();

    virtual void setItemData(TextItem *textItem);

    virtual void setItemData();

    virtual void saveItem();

    virtual void saveItemDisconnect();

    void setCreationAvailable(bool enabled);

    virtual TextItem *createItem();

//    virtual void setX();
//
//    virtual void setY();
//
//    virtual void setWidth();
//
//    virtual void setHeight();
//
//    virtual void setText();

signals:

    void itemCreated(TextItem *textItem);

    void itemUpdated(TextItem *textItem);

    void itemRemoved(TextItem *textItem);

protected:
    TextItem *item_ = nullptr;
    bool creationAvailable = false;
};


#endif //DATAVIEWPANEL_H
