#include <QtCore/QFileInfo>
#include <QtCore/QDir>

#include <QtGui/QImageReader>

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QMessageBox>

#include "ImageViewer.h"


IVInfoPanel::IVInfoPanel(ImageViewer *viewerTab) : InfoPanel() {
    imageViewer_ = viewerTab;
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
}


////////////////////


IVDataViewPanel::IVDataViewPanel(ImageViewer *viewerTab) : DataViewPanel() {
    imageViewer_ = viewerTab;
    creationAvailable = true;
}

void IVDataViewPanel::removeItem() {
    if (imageViewer_)
        imageViewer_->removeItem(item_);
}

void IVDataViewPanel::itemUpdated() {
    DataViewPanel::itemUpdated();
    if (imageViewer_) imageViewer_->refreshItem(item_);
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

TextItem *IVDataViewPanel::createItem() {
    TextItem *textItem = DataViewPanel::createItem();
    if (imageViewer_)
        imageViewer_->addItem(textItem);
    setItem(textItem);
    return textItem;
}


////////////////////


ImageViewer *ImageViewer::loadFile(const QString &path) {
    QImageReader reader(path);
    reader.setAutoTransform(true);
    QImage image = reader.read();
    if (image.isNull()) {
        QMessageBox msg(QMessageBox::Warning,
                        tr("Input error"),
                        tr("Cannot load %1").arg(QDir::toNativeSeparators(path)),
                        QMessageBox::StandardButton::Abort);
        msg.setDetailedText(reader.errorString());
        msg.exec();
        return nullptr;
    } else {
        auto imageViewer = new ImageViewer();
        imageViewer->setImage(image);
        imageViewer->setFileInfo(path);
//        imageViewer->setWindowTitle(QFileInfo(path).fileName());
        return imageViewer;
    }
}


ImageViewer::ImageViewer(QWidget *parent) : Tab(parent) {
    setContentsMargins(0, 0, 0, 0);

    imageView = new ImageView();
    setLayout(new QVBoxLayout());
    layout()->addWidget(imageView);

    infoPanel = new IVInfoPanel(this);
    dataListPanel = new IVDataListPanel(this);
    dataViewPanel = new IVDataViewPanel(this);
}

ImageViewer::~ImageViewer() {
    while (!items_.empty())
        delete items_.takeLast();
}

QList<QWidget *> ImageViewer::getPanels() {
    return QList<QWidget *>{infoPanel, dataListPanel, dataViewPanel};
}

bool ImageViewer::isSaveAvailable() { return imageView && imageView->isSaveAvailable(); }

void ImageViewer::clear() {
    if (dataListPanel)
        dataListPanel->clear();
    if (dataViewPanel)
        dataViewPanel->setItem(nullptr);
    while (!items_.empty())
        delete items_.takeLast();
    if (infoPanel)
        infoPanel->refresh();
}

bool ImageViewer::isClearAvailable() { return !items_.empty(); }

bool ImageViewer::hasImage() { return imageView && imageView->hasImage(); }

QRect ImageViewer::imageRect() { return imageView->imageRect(); }

void ImageViewer::setImage(const QImage &newImage) {
    clear();
    imageView->setImage(newImage);
}

QString ImageViewer::fileInfo() { return filePath; }

void ImageViewer::setFileInfo(const QString &path) {
    filePath = path;
    setWindowTitle(QFileInfo(filePath).fileName());
    if (infoPanel)
        infoPanel->refresh();
}

void ImageViewer::addItem(TextItem *textItem) {
    items_.append(textItem);
    if (dataListPanel)
        dataListPanel->addItem(textItem);
    if (infoPanel)
        infoPanel->refresh();
}

void ImageViewer::refreshItem(TextItem *textItem) {
    if (dataListPanel)
        dataListPanel->refreshItem(textItem);
    if (dataViewPanel && dataViewPanel->item() == textItem)
        dataViewPanel->refreshItemData();
}

void ImageViewer::removeItem(TextItem *textItem) {
    if (dataListPanel)
        dataListPanel->removeItem(textItem);
    if (dataViewPanel && dataViewPanel->item() == textItem)
        dataViewPanel->setItem(nullptr);
    items_.removeOne(textItem);
    delete textItem;
    if (infoPanel)
        infoPanel->refresh();
}

int ImageViewer::itemsCount() { return items_.size(); }