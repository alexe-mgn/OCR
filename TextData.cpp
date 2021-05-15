#include <algorithm>

#include <QtCore/QTextStream>

#include <QtWidgets/QStyle>

#include "TextData.h"


QString itemsToCSV(const QList<TextItem *> &items) {
    char d = ',';
    char n = '\n';
    QString csv;
    QTextStream stream(&csv);
    stream << QList<QString>{"x", "y", "width", "height", "text"}.join(d) << n;
    for (TextItem *item : items) {
        stream << item->pos().x()
               << d << item->pos().y()
               << d << item->size().width()
               << d << item->size().height()
               << d << item->text()
               << n;
    }
    return csv;
}

QString itemsToText(const QList<TextItem *> &items) {
    QString text;
    QTextStream stream(&text);
    QList<TextItem *> sItems = items;
    std::sort(sItems.begin(), sItems.end(), [](TextItem *a, TextItem *b) {
        return a->pos().y() < b->pos().y();
    });
    int n, skip, y, height;
    while (!sItems.empty()) {
        skip = 0, y = 0;
        height = 0;
        n = 0;
        QList<TextItem *> line;
        while (skip < sItems.count()) {
            TextItem *item = sItems[skip];
            if (line.empty() || abs(item->rect().center().y() - y) <= height * 0.5) {
                y = (n * y + item->rect().center().y()) / (n + 1);
                height = (n * height + item->size().height()) / (n + 1);
                sItems.removeOne(item);
                line.append(item);
                ++n;
            } else
                ++skip;
        }
        std::sort(line.begin(), line.end(), [] (TextItem *a, TextItem *b) {
            return a->pos().x() < b->pos().x();
        });
        while (!line.empty()) {
            stream << line.takeFirst()->text();
            if (!line.empty())
                stream << ' ';
        }
        if (n > 0)
            stream << '\n';
    }
    return text;
}


////////////////////


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
    m_press = true;
    QPushButton::mousePressEvent(ev);
}

void TextItemWidget::mouseMoveEvent(QMouseEvent *ev) {
    if (m_press) {
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
}

void TextItemWidget::mouseReleaseEvent(QMouseEvent *ev) {
    if (dragging) {
        dragging = false;
        proxy_->setCursor(Qt::ArrowCursor);
    } else
        QPushButton::mouseReleaseEvent(ev);
    m_press = false;
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