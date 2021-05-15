#ifndef TEXTDATA_H
#define TEXTDATA_H


#include <QtCore/QString>
#include <QtCore/QLineF>
#include <QtCore/QRect>
#include <QtCore/QList>

#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QGraphicsProxyWidget>


class TextItem {

public:
    explicit TextItem() {};

    explicit TextItem(const QRect &rect, const QString &text) : TextItem() {
        setRect(rect);
        setText(text);
    }

    QRect rect() { return rect_; }

    void setRect(const QRect &rect) { rect_ = rect; }

    QPoint pos() { return rect_.topLeft(); }

    void setPos(const QPoint &pos) { rect_.setTopLeft(pos); }

    void setPos(int x, int y) {
        rect_.setLeft(x);
        rect_.setTop(y);
    }

    void setX(int x) { rect_.setLeft(x); }

    void setY(int y) { rect_.setTop(y); }

    QSize size() { return rect_.size(); }

    void setSize(const QSize &size) { rect_.setSize(size); }

    void setSize(int w, int h) {
        rect_.setWidth(w);
        rect_.setHeight(h);
    }

    void setWidth(int w) { rect_.setWidth(w); }

    void setHeight(int h) { rect_.setHeight(h); }

    QString text() { return text_; }

    void setText(const QString &text) { text_ = text; }

protected:
    QRect rect_;
    QString text_;
};


////////////////////


QString itemsToCSV(const QList<TextItem *> &items);

QString itemsToText(const QList<TextItem *> &items);


////////////////////


class TextItemWidget : public QPushButton {
Q_OBJECT

public:
    explicit TextItemWidget();

    explicit TextItemWidget(TextItem *textItem);

    void refresh();

    TextItem *item();

    void setItem(TextItem *textItem);

    void updateItem();

    QGraphicsProxyWidget *proxy();

signals:

    void itemUpdated(TextItem *textItem);

protected:

    void mousePressEvent(QMouseEvent *ev) override;

    void mouseMoveEvent(QMouseEvent *ev) override;

    void mouseReleaseEvent(QMouseEvent *ev) override;

protected:
    TextItem *item_ = nullptr;
    QGraphicsProxyWidget *proxy_ = nullptr;

    bool m_press = false;
    bool dragging = false;
    QLineF shift;
};


////////////////////

/*
class ConnectedTextItem : public TextItem {
public:
    using TextItem::TextItem;

    TextItemWidget *widget() { return widget_; }

    void setWidget(TextItemWidget *newWidget);

    TextListItem *listItem() { return listItem_; }

    void setListItem(TextListItem *newListItem);

    void update() override;

    void remove() override;

protected:
    TextItemWidget *widget_ = nullptr;
    TextListItem *listItem_ = nullptr;
};
*/


#endif //TEXTDATA_H
