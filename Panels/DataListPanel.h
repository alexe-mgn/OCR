#ifndef DATALISTPANEL_H
#define DATALISTPANEL_H


#include <QtCore/QList>

#include <QtWidgets/QWidget>

#include "TextData.h"
#include "ui_TextListItem.h"
#include "ui_DataListPanel.h"


class DataListPanel;

class TextListItem : public QWidget, public Ui::TextListItem {
Q_OBJECT

//    friend class DataListPanel;

public:
    explicit TextListItem();

    explicit TextListItem(TextItem *textItem);

    TextItem *item();

public slots:

    virtual void refresh();

    void setItem(TextItem *textItem);

    virtual void setIndex(int index);

signals:

    void clicked();

    void removeClicked();

protected:
    TextItem *item_ = nullptr;
};


////////////////////


class DataListPanel : public QWidget, public Ui::DataListPanel {
Q_OBJECT

public:
    explicit DataListPanel(QWidget *parent = nullptr);

    virtual TextListItem *widget(TextItem *textItem);

    [[nodiscard]] bool isCreationAvailable() const;

public slots:

    virtual void sortItems();

    virtual void addItem(TextItem *textItem);

    virtual void refreshItem(TextItem *textItem);

    virtual TextItem *createItem();

    virtual void removeItem(TextItem *textItem);

    virtual void reindex();

    virtual void clear();

    void setCreationAvailable(bool enabled);

signals:

    void itemAdded(TextItem *textItem);

    void itemClicked(TextItem *textItem);

    void itemRemoveClicked(TextItem *textItem);

    void itemRemoved(TextItem *textItem);

protected:
    QList<TextListItem *> listItems;

    bool creationAvailable = false;

    virtual void connectListItem(TextListItem *listItem);

protected slots:

    void emitSenderClicked();

    void emitSenderRemoveClicked();
};


#endif //DATALISTPANEL_H
