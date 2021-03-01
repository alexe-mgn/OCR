#include <QtWidgets/QStyle>

#include "ToolPanels.h"


InfoPanel::InfoPanel(QWidget *parent) : QWidget(parent) {
    setupUi(this);
}


////////////////////


TextListItem::TextListItem() {}

TextListItem::TextListItem(TextItem *textItem) {
    setItem(textItem);
}

void TextListItem::refresh() {
    if (item_)
        setText(item_->text());
}

TextItem *TextListItem::item() {
    return item_;
}

void TextListItem::setItem(TextItem *textItem) {
    item_ = textItem;
    refresh();
}


////////////////////


DataListPanel::DataListPanel(QWidget *parent) : QWidget(parent) {
    setupUi(this);
}

TextListItem *DataListPanel::widget(TextItem *textItem) {
    TextListItem *listItem = nullptr;
    for (auto i = listItems.begin(); i != listItems.end() && listItem == nullptr; ++i)
        if ((*i)->item() == textItem)
            listItem = *i;
    return listItem;
}

void DataListPanel::refreshItem(TextItem *textItem) {
    widget(textItem)->refresh();
}

void DataListPanel::addItem(TextItem *textItem) {
    auto listItem = new TextListItem(textItem);
    listItems.append(listItem);
    itemsLayout->addWidget(listItem);
}

void DataListPanel::removeItem(TextItem *textItem) {
    TextListItem *listItem = widget(textItem);
    itemsLayout->removeWidget(listItem);
    listItems.removeOne(listItem);
    listItem->deleteLater();
}

void DataListPanel::clear() {
    while (!listItems.empty())
        listItems.takeLast()->deleteLater();
}


////////////////////


DataViewPanel::DataViewPanel(QWidget *parent) : QWidget(parent) {
    setupUi(this);
    connect(posXValue, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &DataViewPanel::setX);
    connect(posYValue, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &DataViewPanel::setY);
    connect(sizeXValue, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &DataViewPanel::setWidth);
    connect(sizeYValue, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &DataViewPanel::setHeight);
    connect(textValue, &QLineEdit::editingFinished, this, &DataViewPanel::setText);
}

TextItem *DataViewPanel::item() { return item_; }

void DataViewPanel::setItem(TextItem *textItem) {
    item_ = textItem;
    refresh();
}

void DataViewPanel::removeItem() {
    setItem(nullptr);
}

void DataViewPanel::setEditable(bool enabled) {
    posXValue->setEnabled(enabled);
    posYValue->setEnabled(enabled);
    sizeXValue->setEnabled(enabled);
    sizeYValue->setEnabled(enabled);
    textValue->setEnabled(enabled);
    deleteButton->setEnabled(enabled);
}

void DataViewPanel::refresh() {
    disconnect(deleteButton, &QToolButton::clicked, this, nullptr);

    setEditable(item_ || isCreationAvailable());

    bool creationMode = isCreationAvailable() && !item_;
    deleteButton->setIcon(deleteButton->style()->standardIcon(
            creationMode ? QStyle::SP_DialogSaveButton : QStyle::SP_DialogCloseButton
    ));
    if (creationMode)
        connect(deleteButton, &QToolButton::clicked,
                this, &DataViewPanel::createItem);
    else
        connect(deleteButton, &QToolButton::clicked,
                this, &DataViewPanel::removeItem);

    refreshRanges();
    refreshItemData();
}

void DataViewPanel::refreshItemData() {
    if (item_) {
        QPoint pos = item_->pos();
        posXValue->setValue(pos.x());
        posYValue->setValue(pos.y());
        QSize size = item_->size();
        sizeXValue->setValue(size.width());
        sizeYValue->setValue(size.height());
        textValue->setText(item_->text());
    } else {
        posXValue->setValue(0);
        posYValue->setValue(0);
        sizeXValue->setValue(10);
        sizeYValue->setValue(10);
        textValue->setText("");
    }
}

void DataViewPanel::setItemData(TextItem *textItem) {
    textItem->setX(posXValue->value());
    textItem->setY(posYValue->value());
    textItem->setWidth(sizeXValue->value());
    textItem->setHeight(sizeYValue->value());
    textItem->setText(textValue->text());
}

void DataViewPanel::setItemData() { setItemData(item_); }

bool DataViewPanel::isCreationAvailable() { return creationAvailable; }

void DataViewPanel::setCreationAvailable(bool enabled) { creationAvailable = enabled; }

TextItem *DataViewPanel::createItem() {
    auto textItem = new TextItem;
    setItemData(textItem);
    return textItem;
}

void DataViewPanel::setX() {
    if (item_) {
        item_->setX(posXValue->value());
        itemUpdated();
    }
}

void DataViewPanel::setY() {
    if (item_) {
        item_->setY(posYValue->value());
        itemUpdated();
    }
}

void DataViewPanel::setWidth() {
    if (item_) {
        item_->setWidth(sizeXValue->value());
        itemUpdated();
    }
}

void DataViewPanel::setHeight() {
    if (item_) {
        item_->setHeight(sizeYValue->value());
        itemUpdated();
    }
}

void DataViewPanel::setText() {
    if (item_) {
        item_->setText(textValue->text());
        itemUpdated();
    }
}