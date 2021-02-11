#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QtWidgets/QAbstractScrollArea>
#include <QtWidgets/QLabel>


class ImageView : public QAbstractScrollArea {
Q_OBJECT
protected:
    void wheelEvent(QWheelEvent *event) override;

protected:
    double scaleFactor = 1;

    QImage *image = nullptr;

public:
    QLabel label;
    QRect viewRect = QRect(0, 0, 0, 0);

    explicit ImageView(QWidget *parent = nullptr);

    void setImage(QImage &newImage);

    void updateView();

    void updateSize();

    void scrollContentsBy(int dx, int dy) override;

    void resizeEvent(QResizeEvent *event) override;

    void setScale(double scale);
};


#endif
