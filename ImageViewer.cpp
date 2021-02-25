#include <QtWidgets/QVBoxLayout>

#include "ImageViewer.h"


ImageViewer::ImageViewer(QWidget *parent) : Tab(parent) {
    setContentsMargins(0, 0, 0, 0);

    imageView = new ImageView();
    setLayout(new QVBoxLayout());
    layout()->addWidget(imageView);

    infoPanel = new InfoPanel();
    dataListPanel = new DataListPanel();
    dataViewPanel = new QWidget();
}

#include <QtDebug>
ImageViewer::~ImageViewer() {
//    infoPanel->deleteLater();
//    dataListPanel->deleteLater();
//    dataViewPanel->deleteLater();
//    imageView->deleteLater();
}

QList<QWidget *> ImageViewer::getPanels() {
    return QList<QWidget *>{infoPanel, dataListPanel, dataViewPanel};
}

bool ImageViewer::isSaveAvailable() {
    return imageView && imageView->isSaveAvailable();
}

void ImageViewer::setImage(QImage &newImage) {
    imageView->setImage(newImage);
}