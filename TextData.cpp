#include <QtWidgets/QStyle>

#include "TextData.h"


TextItemWidget::TextItemWidget() : QPushButton() {
    proxy_ = new QGraphicsProxyWidget();
    proxy_->setWidget(this);
    proxy_->setMinimumSize(0, 0);

    setStyleSheet("border: 1px solid rgb(0, 0, 255); background: rgba(0, 0, 0, 0)");

    proxy_->setFlag(QGraphicsProxyWidget::GraphicsItemFlag::ItemIsSelectable);
//    proxy_->setFlag(QGraphicsProxyWidget::GraphicsItemFlag::ItemIsMovable);
//    proxy_->setFlag(QGraphicsProxyWidget::GraphicsItemFlag::ItemSendsGeometryChanges);
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

void TextItemWidget::updateItem() {
    item_->setRect(geometry());
    emit itemUpdated(item_);
}

QGraphicsProxyWidget *TextItemWidget::proxy() { return proxy_; }

void TextItemWidget::mousePressEvent(QMouseEvent *ev) {
    shift.setP1(proxy_->mapToScene(ev->pos()));
    QPushButton::mousePressEvent(ev);
}

void TextItemWidget::mouseMoveEvent(QMouseEvent *ev) {
    if (dragging) {
        move((proxy_->mapToScene(ev->pos()) - shift.p2() + shift.p1()).toPoint());
        updateItem();
    } else {
        shift.setP2(proxy_->mapToScene(ev->pos()));
        if (shift.length() > 3) {
            dragging = true;
            shift.setP1(pos());
            proxy_->setCursor(Qt::ClosedHandCursor);
            setDown(false);
        }
    }
}

void TextItemWidget::mouseReleaseEvent(QMouseEvent *ev) {
    if (dragging) {
        dragging = false;
        proxy_->setCursor(Qt::ArrowCursor);
    } else
        QPushButton::mouseReleaseEvent(ev);
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