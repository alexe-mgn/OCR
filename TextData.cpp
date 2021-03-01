#include <QtWidgets/QStyle>

#include "TextData.h"



////////////////////


TextItemWidget::TextItemWidget() : QPushButton() {
    setStyleSheet("border: 1px solid rgb(0, 0, 255); background: rgba(0, 0, 0, 0)");
}

TextItemWidget::TextItemWidget(TextItem *textItem) : TextItemWidget() {
    setItem(textItem);
}

void TextItemWidget::refresh() {
    if (item_) {
        setGeometry(item_->rect());
        setText(item_->text());
    }
}

TextItem *TextItemWidget::item() { return item_; }

void TextItemWidget::setItem(TextItem *textItem) {
    item_ = textItem;
    refresh();
}


////////////////////

/*
void ConnectedTextItem::setWidget(TextItemWidget *newWidget) {
    if (widget_) widget_->setItem(nullptr);
    widget_ = newWidget;
    if (widget_) widget_->refresh();
}

void ConnectedTextItem::setListItem(TextListItem *newListItem) {
    if (listItem_) listItem_->setItem(nullptr);
    listItem_ = newListItem;
    if (listItem_) listItem_->refresh();
}

void ConnectedTextItem::update() {
    TextItem::update();
    if (widget_)
        widget_->refresh();
    if (listItem_)
        listItem_->refresh();
}

void ConnectedTextItem::remove() {
    if (listItem_)
        listItem_->deleteLater();
    if (widget_)
        widget_->deleteLater();
    TextItem::remove();
}
*/