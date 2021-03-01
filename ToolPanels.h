#ifndef TOOLPANELS_H
#define TOOLPANELS_H

#include <QtCore/QList>

#include <QtWidgets/QWidget>

#include "TextData.h"
#include "ui_InfoPanel.h"
#include "ui_DataListPanel.h"
#include "ui_DataViewPanel.h"


class InfoPanel : public QWidget, public Ui::InfoPanel {
Q_OBJECT

public:
    explicit InfoPanel(QWidget *parent = nullptr);

    virtual void refresh() {};
};


////////////////////


class TextListItem : public QPushButton {
Q_OBJECT

public:
    explicit TextListItem();

    explicit TextListItem(TextItem *textItem);

    void refresh();

    TextItem *item();

    void setItem(TextItem *textItem);

protected:
    TextItem *item_ = nullptr;
};


////////////////////


class DataListPanel : public QWidget, public Ui::DataListPanel {
Q_OBJECT

public:
    explicit DataListPanel(QWidget *parent = nullptr);

    virtual TextListItem *widget(TextItem *textItem);

    virtual void refreshItem(TextItem *textItem);

    virtual void addItem(TextItem *textItem);

    virtual void removeItem(TextItem *textItem);

    virtual void clear();

protected:
    QList<TextListItem *> listItems;
};


////////////////////


class DataViewPanel : public QWidget, public Ui::DataViewPanel {
Q_OBJECT

public:
    explicit DataViewPanel(QWidget *parent = nullptr);

    virtual TextItem *item();

    virtual void setItem(TextItem *textItem);

    virtual void removeItem();

    virtual void setEditable(bool enabled);

    virtual void itemUpdated() {};

    virtual void refresh();

    virtual void refreshRanges() {};

    virtual void refreshItemData();

    virtual void setItemData(TextItem *textItem);

    virtual void setItemData();

    virtual bool isCreationAvailable();

    virtual void setCreationAvailable(bool enabled);

    virtual TextItem *createItem();

    virtual void setX();

    virtual void setY();

    virtual void setWidth();

    virtual void setHeight();

    virtual void setText();

protected:
    TextItem *item_ = nullptr;
    bool creationAvailable = false;
};


#endif //TOOLPANELS_H
