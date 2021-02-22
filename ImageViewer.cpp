#include <QtWidgets/QVBoxLayout>

#include "ImageViewer.h"


ImageViewer::ImageViewer(QWidget *parent) : Tab(parent) {
    setContentsMargins(0, 0, 0, 0);

    imageView = new ImageView();
    setLayout(new QVBoxLayout());
    layout()->addWidget(imageView);
}

bool ImageViewer::isSaveAvailable() { return imageView && imageView->isSaveAvailable(); }

void ImageViewer::setImage(QImage &newImage) {
    imageView->setImage(newImage);
}