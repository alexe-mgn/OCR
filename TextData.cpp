#include <algorithm>

#include <QtCore/QTextStream>

#include <QtWidgets/QStyle>

#include "TextData.h"


QList<TextItem *> joinLetters(QList<TextItem *> letters) {
    QList<TextItem *> words;
    std::sort(letters.begin(), letters.end(), [](TextItem *a, TextItem *b) {
        return a->pos().x() < b->pos().x();
    });
    int skip;
    while (!letters.empty()) {
        skip = 0;
        QList<TextItem *> word;
        QRect word_rect;
        QRect prev_rect;
        while (skip < letters.count()) {
            TextItem *letter = letters[skip];
            if (word.empty() || (
                    abs(
                            letter->rect().center().y() - (prev_rect = word.last()->rect()).center().y()
                    ) <= prev_rect.height() * 0.5 &&
                    abs(
                            letter->rect().left() - prev_rect.right()
                    ) <= (letter->rect().width() + prev_rect.width()) * 0.14)
                    ) {
                letters.removeOne(letter);
                word_rect = !word.empty() ? word_rect.united(letter->rect()) : letter->rect();
                word.append(letter);
            } else
                ++skip;
        }
        std::sort(word.begin(), word.end(), [](TextItem *a, TextItem *b) {
            return a->pos().x() < b->pos().x();
        });
        QString word_text;
        QTextStream stream(&word_text);
        while (!word.empty())
            stream << word.takeFirst()->text();
        words.append(new TextItem(word_rect, word_text));
    }
    return words;
}

QList<QList<TextItem *>> joinLines(const QList<TextItem *> &items) {
    QList<QList<TextItem *>> lines;
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
        std::sort(line.begin(), line.end(), [](TextItem *a, TextItem *b) {
            return a->pos().x() < b->pos().x();
        });
        lines.append(line);
    }
    return lines;
}

QString itemsToCSV(const QList<TextItem *> &items) {
    char d = ',';
    char n = '\n';
    QString csv;
    QTextStream stream(&csv);
    stream << QList<QString>{"x", "y", "width", "height", "text"}.join(d) << n;
    for (TextItem *item: items) {
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
    QList<QList<TextItem *>> lines = joinLines(items);
    while (!lines.empty()) {
        int n = 0;
        QList<TextItem *> line = lines.takeFirst();
        while (!line.empty()) {
            stream << line.takeFirst()->text();
            if (!line.empty())
                stream << ' ';
            ++n;
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

    setStyleSheet("border: 2px solid rgb(0, 0, 255); background: rgba(0, 0, 0, 0)");

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
//        setText(item_->text());
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
