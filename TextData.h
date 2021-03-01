#ifndef TEXTDATA_H
#define TEXTDATA_H

#include <QtCore/QRect>

#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>


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


class TextItemWidget : public QPushButton {
Q_OBJECT

public:
    explicit TextItemWidget();

    explicit TextItemWidget(TextItem *textItem);

    void refresh();

    TextItem *item();

    void setItem(TextItem *textItem);

protected:
    TextItem *item_ = nullptr;
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
