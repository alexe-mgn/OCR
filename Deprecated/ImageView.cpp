#include <QtGui/QPaintEvent>
#include <QtGui/QResizeEvent>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QScrollBar>

#include "ImageView.h"


ImageView::ImageView(QWidget *parent) : QAbstractScrollArea(parent) {
    viewport()->setLayout(new QHBoxLayout());
    viewport()->layout()->setContentsMargins(0, 0, 0, 0);
    viewport()->layout()->addWidget(&label);

    setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOn);
    setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOn);

    label.setAlignment(Qt::AlignCenter);
    label.setScaledContents(false);

    setImage(*(new QImage("C:\\Users\\admin\\Desktop\\котик.jpg", "jpg")));
    for (int i = 0; i < image->height(); ++i)
        image->setPixelColor(0, i, QColor("red"));
    for (int i = 0; i < image->height(); ++i)
        image->setPixelColor(image->width() - 1, i, QColor("red"));
    for (int i = 0; i < image->width(); ++i)
        image->setPixelColor(i, 0, QColor("red"));
    for (int i = 0; i < image->width(); ++i)
        image->setPixelColor(i, image->height() - 1, QColor("red"));
}

void ImageView::updateView() {
    viewRect.moveTo(
            horizontalScrollBar()->value(),
            verticalScrollBar()->value()
    );
    label.setPixmap(QPixmap::fromImage(
            image->copy(viewRect)
    ).scaled(
            label.size(),
            Qt::KeepAspectRatio
    ));
}

void ImageView::updateSize() {
    QSize iSize = image->size();
    QPoint center = viewRect.center();
    viewRect.setSize(
            (label.size().scaled(iSize, Qt::KeepAspectRatioByExpanding) / scaleFactor).boundedTo(iSize)
    );
    viewRect.moveCenter(center);

    horizontalScrollBar()->setRange(0, iSize.width() - viewRect.width());
    verticalScrollBar()->setRange(0, iSize.height() - viewRect.height());
    horizontalScrollBar()->setPageStep(viewRect.width());
    verticalScrollBar()->setPageStep(viewRect.height());

    horizontalScrollBar()->blockSignals(true);
    horizontalScrollBar()->setValue(viewRect.x());
    horizontalScrollBar()->blockSignals(false);

    verticalScrollBar()->blockSignals(true);
    verticalScrollBar()->setValue(viewRect.y());
    verticalScrollBar()->blockSignals(false);

    updateView();
}

void ImageView::setScale(double scale) {
    scaleFactor = scale;
    updateSize();
}

void ImageView::setImage(QImage &newImage) {
    delete image;
    image = new QImage(newImage);
    scaleFactor = 1;
    viewRect.moveTo(0, 0);
    viewRect.setSize(image->size());
    updateSize();
}

void ImageView::scrollContentsBy(int dx, int dy) {
//    QAbstractScrollArea::scrollContentsBy(dx, dy);
    updateView();
}

void ImageView::resizeEvent(QResizeEvent *event) {
    QAbstractScrollArea::resizeEvent(event);
    updateSize();
}

void ImageView::wheelEvent(QWheelEvent *event) {
//    QAbstractScrollArea::wheelEvent(event);
    if (event->angleDelta().y() >= 0)
        setScale(scaleFactor * 1.25);
    else
        setScale(scaleFactor / 1.25);
}
