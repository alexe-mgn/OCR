#include <QtCore/QSignalBlocker>

#include <QtWidgets/QStyle>

#include "DataViewPanel.h"


DataViewPanel::DataViewPanel(QWidget *parent) : QWidget(parent) {
    setupUi(this);
    saveButton->setIcon(saveButton->style()->standardIcon(QStyle::SP_DialogSaveButton));

//    auto f = static_cast<void (DataViewPanel::*)()>(&DataViewPanel::saveItem);
    connect(posXValue, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &DataViewPanel::saveItem);
    connect(posYValue, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &DataViewPanel::saveItem);
    connect(sizeXValue, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &DataViewPanel::saveItem);
    connect(sizeYValue, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &DataViewPanel::saveItem);
    connect(textValue, &QLineEdit::editingFinished, this, &DataViewPanel::saveItem);
    connect(saveButton, &QToolButton::clicked,
            this, &DataViewPanel::saveItemDisconnect);
}

TextItem *DataViewPanel::item() { return item_; }

void DataViewPanel::setItem(TextItem *textItem) {
    item_ = textItem;
    refresh();
}

void DataViewPanel::removeItem() {
    TextItem *i = item_;
    setItem(nullptr);
    emit itemRemoved(i);
}

void DataViewPanel::setEditable(bool enabled) {
    posXValue->setEnabled(enabled);
    posYValue->setEnabled(enabled);
    sizeXValue->setEnabled(enabled);
    sizeYValue->setEnabled(enabled);
    textValue->setEnabled(enabled);
    creationButton->setEnabled(enabled);
}

void DataViewPanel::refresh() {
    disconnect(creationButton, &QToolButton::clicked, this, nullptr);

    setEditable(item_ || isCreationAvailable());
    saveButton->setEnabled(item_);

    bool creationMode = isCreationAvailable() && !item_;
    creationButton->setIcon(creationButton->style()->standardIcon(
            creationMode ? QStyle::SP_FileDialogNewFolder : QStyle::SP_DialogCloseButton
    ));
    if (creationMode)
        connect(creationButton, &QToolButton::clicked,
                this, &DataViewPanel::createItem);
    else
        connect(creationButton, &QToolButton::clicked,
                this, &DataViewPanel::removeItem);

    refreshRanges();
    refreshItemData();
}

void DataViewPanel::refreshItemData() {
    const QSignalBlocker posXValueblocker(posXValue);
    const QSignalBlocker posYValueblocker(posYValue);
    const QSignalBlocker sizeXValueblocker(sizeXValue);
    const QSignalBlocker sizeYValueblocker(sizeYValue);
    const QSignalBlocker textValueblocker(textValue);
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

void DataViewPanel::setItemData() {
    setItemData(item_);
    emit itemUpdated(item_);
}

void DataViewPanel::saveItem() {
    if (item_)
        setItemData();
}

void DataViewPanel::saveItemDisconnect() {
    saveItem();
    setItem(nullptr);
}

bool DataViewPanel::isCreationAvailable() { return creationAvailable; }

void DataViewPanel::setCreationAvailable(bool enabled) { creationAvailable = enabled; }

TextItem *DataViewPanel::createItem() {
    auto textItem = new TextItem;
    setItemData(textItem);
    emit itemCreated(textItem);
    return textItem;
}

//void DataViewPanel::setX() {
//    if (item_) {
//        item_->setX(posXValue->value());
//        itemUpdated();
//    }
//}
//
//void DataViewPanel::setY() {
//    if (item_) {
//        item_->setY(posYValue->value());
//        itemUpdated();
//    }
//}
//
//void DataViewPanel::setWidth() {
//    if (item_) {
//        item_->setWidth(sizeXValue->value());
//        itemUpdated();
//    }
//}
//
//void DataViewPanel::setHeight() {
//    if (item_) {
//        item_->setHeight(sizeYValue->value());
//        itemUpdated();
//    }
//}
//
//void DataViewPanel::setText() {
//    if (item_) {
//        item_->setText(textValue->text());
//        itemUpdated();
//    }
//}