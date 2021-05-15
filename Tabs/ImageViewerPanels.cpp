#include <QtCore/QRect>

#include "Tabs/ImageViewerPanels.h"
#include "Tabs/ImageViewer.h"


IVInfoPanel::IVInfoPanel(ImageViewer *viewerTab) : InfoPanel() {
    imageViewer_ = viewerTab;
    connect(scanButton, &QPushButton::clicked, imageViewer_, &ImageViewer::scan);
}

void IVInfoPanel::refresh() {
    InfoPanel::refresh();
    if (imageViewer_) {
        fileValue->setText(imageViewer_->fileInfo());
        wordsCountValue->setText(QString::number(imageViewer_->itemsCount()));
    }
}


////////////////////


IVDataListPanel::IVDataListPanel(ImageViewer *viewerTab) : DataListPanel() {
    imageViewer_ = viewerTab;
    setCreationAvailable(true);
    connect(this, &IVDataListPanel::itemAdded, imageViewer_, &ImageViewer::addItem);
    connect(this, &IVDataListPanel::itemClicked, imageViewer_, &ImageViewer::selectItem);
    connect(this, &IVDataListPanel::itemRemoved, imageViewer_, &ImageViewer::removeItem);
}

TextItem *IVDataListPanel::createItem() {
    if (imageViewer_ && imageViewer_->dataViewPanel)
        return imageViewer_->dataViewPanel->createItem();
    else
        return DataListPanel::createItem();
}


////////////////////


IVDataViewPanel::IVDataViewPanel(ImageViewer *viewerTab) : DataViewPanel() {
    imageViewer_ = viewerTab;
    setCreationAvailable(true);
    connect(this, &IVDataViewPanel::itemUpdated, imageViewer_, &ImageViewer::refreshItem);
    connect(this, &IVDataViewPanel::itemCreated, imageViewer_, &ImageViewer::addItem);
//    connect(this, &IVDataViewPanel::itemCreated, imageViewer_, &ImageViewer::selectItem);
    connect(this, &IVDataViewPanel::itemRemoved, imageViewer_, &ImageViewer::removeItem);
}

void IVDataViewPanel::refreshRanges() {
    DataViewPanel::refreshRanges();
    if (imageViewer_) {
        QRect imageRect = imageViewer_->imageRect();
        posXValue->setRange(0, imageRect.width());
        posYValue->setRange(0, imageRect.height());
        sizeXValue->setRange(0, imageRect.width());
        sizeYValue->setRange(0, imageRect.height());
    }
}
