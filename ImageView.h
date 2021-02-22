#ifndef GRAPHICSIMAGEVIEW_H
#define GRAPHICSIMAGEVIEW_H


#include <QtGui/QWheelEvent>
#include <QtGui/QImage>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGraphicsPixmapItem>


class ImageView : public QGraphicsView {
Q_OBJECT

public:
    explicit ImageView(QWidget *parent = nullptr);

    ~ImageView() override;

    void reset();

    bool isSaveAvailable();

    void setImage(QImage &newImage);

    void resetScale();

    void setScale(double newScaleFactor);

    void zoomIn(double coef = 1.25);

    void zoomOut(double coef = 1.25);

    QGraphicsPixmapItem pixmapItem;

protected:
    QImage *image = nullptr;
    double scaleFactor = 1;

    void wheelEvent(QWheelEvent *event) override;

    void keyPressEvent(QKeyEvent *event) override;
};


#endif
