#ifndef GRAPHICSIMAGEVIEW_H
#define GRAPHICSIMAGEVIEW_H

#include <QtGui/QKeyEvent>
#include <QtGui/QWheelEvent>
#include <QtGui/QImage>

#include <QtWidgets/QWidget>
#include <QtWidgets/QGraphicsPixmapItem>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsView>


class ImageView : public QGraphicsView {
Q_OBJECT

public:
    explicit ImageView(QWidget *parent = nullptr);

    ~ImageView() override;

    void reset();

    bool isSaveAvailable() { return (bool) image_; };

    bool hasImage() { return (bool) image_; };

    const QImage &image();

    void setImage(const QImage &newImage);

    [[nodiscard]] QRect imageRect() const {
        return QRect(QPoint(0, 0), pixmapItem.boundingRect().size().toSize());
    }

    void resetScale();

    void setScale(double newScaleFactor);

    void zoomIn(double coef = 1.25);

    void zoomOut(double coef = 1.25);

    QGraphicsPixmapItem pixmapItem;

protected:
    QImage *image_ = nullptr;
    double scaleFactor = 1;

    void wheelEvent(QWheelEvent *event) override;

    void keyPressEvent(QKeyEvent *event) override;
};


#endif
