#include "ImageView.h"


ImageView::ImageView(QWidget *parent) : QGraphicsView(parent) {
    setScene(new QGraphicsScene());
    scene()->addItem(&pixmapItem);

    setResizeAnchor(ViewportAnchor::AnchorViewCenter);
    setDragMode(DragMode::ScrollHandDrag);

//    setImage(*(new QImage("C:\\Users\\admin\\Desktop\\котик.jpg", "jpg")));
}

ImageView::~ImageView() {
    delete image_;
    QGraphicsScene *sc = scene();
    setScene(nullptr);
    sc->deleteLater();
}

void ImageView::reset() {
    resetTransform();
    scaleFactor = 1;
}

void ImageView::setImage(const QImage &newImage) {
    delete image_;
    image_ = new QImage(newImage);
    pixmapItem.setPixmap(QPixmap::fromImage(*image_));
    reset();
    setSceneRect(pixmapItem.sceneBoundingRect());
}

void ImageView::resetScale() {
    scale(1 / scaleFactor, 1 / scaleFactor);
    scaleFactor = 1;
}

void ImageView::setScale(double newScaleFactor) {
    scale(newScaleFactor / scaleFactor, newScaleFactor / scaleFactor);
    scaleFactor = newScaleFactor;
}

void ImageView::zoomIn(double coef) {
    scale(coef, coef);
    scaleFactor *= coef;
}

void ImageView::zoomOut(double coef) {
    scale(1 / coef, 1 / coef);
    scaleFactor /= coef;
}

void ImageView::wheelEvent(QWheelEvent *event) {
//    QGraphicsView::wheelEvent(event);
    QPointF sp = mapToScene(event->pos());
    if (event->angleDelta().y() >= 0)
        zoomIn();
    else
        zoomOut();
    centerOn(mapToScene(viewport()->rect().center()) + sp - mapToScene(event->pos()));
}

void ImageView::keyPressEvent(QKeyEvent *event) {
//    QGraphicsView::keyPressEvent(event);
}
