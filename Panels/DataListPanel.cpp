#include <QtCore/QString>

#include <QtWidgets/QStyle>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QToolButton>

#include "DataListPanel.h"


TextListItem::TextListItem() : QWidget() {
    setupUi(this);
    deleteButton->setIcon(deleteButton->style()->standardIcon(QStyle::SP_DialogCloseButton));
    connect(mainButton, &QToolButton::clicked, this, &TextListItem::clicked);
    connect(deleteButton, &QToolButton::clicked, this, &TextListItem::removeClicked);
}

TextListItem::TextListItem(TextItem *textItem) : TextListItem() {
    setItem(textItem);
}

void TextListItem::refresh() {
    if (item_)
        mainButton->setText(item_->text());
}

TextItem *TextListItem::item() {
    return item_;
}

void TextListItem::setItem(TextItem *textItem) {
    item_ = textItem;
    refresh();
}

void TextListItem::setIndex(int index) {
    numLabel->setText(QString::number(index));
}


////////////////////


DataListPanel::DataListPanel(QWidget *parent) : QWidget(parent) {
    setupUi(this);
    connect(this->addButton, &QPushButton::clicked, this, &DataListPanel::createItem);
    connect(this->sortButton, &QPushButton::clicked, this, &DataListPanel::sortItems);
    connect(this, &DataListPanel::itemRemoveClicked, this, &DataListPanel::removeItem);
}

TextListItem *DataListPanel::widget(TextItem *textItem) {
    TextListItem *listItem = nullptr;
    for (auto i = listItems.begin(); i != listItems.end() && listItem == nullptr; ++i)
        if ((*i)->item() == textItem)
            listItem = *i;
    return listItem;
}

bool DataListPanel::isCreationAvailable() const { return creationAvailable; }

void DataListPanel::sortItems() {
    QList<TextItem *> textItems;
    for (TextListItem *listItem: listItems)
        textItems.append(listItem->item());
    QList<QList<TextItem *>> lines = joinLines(textItems);

    int i = 0;
    while (!lines.empty()) {
        QList<TextItem *> line = lines.takeFirst();
        while (!line.empty())
            listItems[i++]->setItem(line.takeFirst());
    }
}

void DataListPanel::connectListItem(TextListItem *listItem) {
    connect(listItem, &TextListItem::clicked, this, &DataListPanel::emitSenderClicked);
    connect(listItem, &TextListItem::removeClicked, this, &DataListPanel::emitSenderRemoveClicked);
}

void DataListPanel::addItem(TextItem *textItem) {
    auto listItem = new TextListItem(textItem);
    listItems.append(listItem);
    itemsLayout->insertWidget(itemsLayout->indexOf(verticalSpacer), listItem);
    listItem->setIndex(listItems.size());
    connectListItem(listItem);
    emit itemAdded(textItem);
}

void DataListPanel::refreshItem(TextItem *textItem) {
    widget(textItem)->refresh();
}

TextItem *DataListPanel::createItem() {
    auto textItem = new TextItem();
    addItem(textItem);
    return textItem;
}

void DataListPanel::removeItem(TextItem *textItem) {
    TextListItem *listItem = widget(textItem);
    listItem->deleteButton->setDisabled(true);
    itemsLayout->removeWidget(listItem);
    listItems.removeOne(listItem);
    listItem->deleteLater();
    reindex();
    emit itemRemoved(textItem);
}

void DataListPanel::reindex() {
    int ind = 0;
    for (auto i: listItems)
        i->setIndex(++ind);
}

void DataListPanel::clear() {
    while (!listItems.empty())
        listItems.takeLast()->deleteLater();
}

void DataListPanel::setCreationAvailable(bool enabled) {
    creationAvailable = enabled;
    addButton->setVisible(enabled);
}

void DataListPanel::emitSenderClicked() {
    emit itemClicked(dynamic_cast<TextListItem *>(sender())->item());
}

void DataListPanel::emitSenderRemoveClicked() {
    emit itemRemoveClicked(dynamic_cast<TextListItem *>(sender())->item());
}
