#include <QtCore/QSignalBlocker>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>

#include <QtGui/QClipboard>
#include <QtGui/QImageReader>

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QMessageBox>

#include "ImageViewer.h"


IVImageView::IVImageView(ImageViewer *viewerTab) : ImageView() {
    imageViewer_ = viewerTab;
    connect(this->scene(), &QGraphicsScene::selectionChanged, imageViewer_, &ImageViewer::imageSelectionChanged);
}

void IVImageView::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::MouseButton::RightButton) {
        if (dragMode() == IVImageView::ScrollHandDrag)
            setDragMode(IVImageView::RubberBandDrag);
        else if (dragMode() == ImageView::RubberBandDrag)
            setDragMode(IVImageView::ScrollHandDrag);
    } else
        ImageView::mousePressEvent(event);
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

    imageView = new IVImageView(this);
    setLayout(new QVBoxLayout());
    layout()->addWidget(imageView);

    infoPanel = new IVInfoPanel(this);
    dataListPanel = new IVDataListPanel(this);
    dataViewPanel = new IVDataViewPanel(this);
}

ImageViewer::~ImageViewer() {
    while (!items.empty())
        delete items.takeLast();
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
    while (!items.empty())
        delete items.takeLast();
    itemWidgets.clear();
    if (infoPanel)
        infoPanel->refresh();
}

bool ImageViewer::isClearAvailable() { return !items.empty(); }

bool ImageViewer::hasImage() { return imageView && imageView->hasImage(); }

QRect ImageViewer::imageRect() { return imageView->imageRect(); }

void ImageViewer::setImage(const QImage &newImage) {
    imageView->setImage(newImage);
    if (dataViewPanel)
        dataViewPanel->refresh();
}

QString ImageViewer::fileInfo() { return filePath; }

void ImageViewer::setFileInfo(const QString &path) {
    filePath = path;
    setWindowTitle(QFileInfo(filePath).fileName());
    if (infoPanel)
        infoPanel->refresh();
}

TextItemWidget *ImageViewer::widget(TextItem *textItem) {
    TextItemWidget *itemWidget = nullptr;
    for (auto i = itemWidgets.begin(); i != itemWidgets.end() && itemWidget == nullptr; ++i)
        if ((*i)->item() == textItem)
            itemWidget = *i;
    return itemWidget;
}

void ImageViewer::addItem(TextItem *textItem) {
    items.append(textItem);
    if (dataListPanel && sender() != dataListPanel) {
        const QSignalBlocker blocker(dataListPanel);
        dataListPanel->addItem(textItem);
    }

    auto widget = new TextItemWidget(textItem);
    connect(widget, &TextItemWidget::itemUpdated, this, &ImageViewer::refreshItem);
    itemWidgets.append(widget);
    imageView->scene()->addItem(widget->proxy());

    if (infoPanel)
        infoPanel->refresh();
}

void ImageViewer::selectItem(TextItem *textItem) {
    if (dataListPanel)
        dataListPanel->selectItem(textItem);
    if (dataViewPanel)
        dataViewPanel->setItem(textItem);
}

void ImageViewer::refreshItem(TextItem *textItem) {
    TextItemWidget *itemWidget;
    if (sender() != (itemWidget = widget(textItem)) || sender() == nullptr)
        itemWidget->refresh();
    if (dataListPanel && sender() != dataListPanel)
        dataListPanel->refreshItem(textItem);
    if (dataViewPanel && textItem && dataViewPanel->item() == textItem)
        dataViewPanel->refreshItemData();
}

void ImageViewer::removeItem(TextItem *textItem) {

    if (dataListPanel && sender() != dataListPanel) {
        const QSignalBlocker blocker(dataListPanel);
        dataListPanel->removeItem(textItem);
    }
    if (dataViewPanel && sender() != dataViewPanel && dataViewPanel->item() == textItem) {
        const QSignalBlocker blocker(dataViewPanel);
        dataViewPanel->removeItem();
    }
    TextItemWidget *itemWidget = widget(textItem);
    itemWidgets.removeOne(itemWidget);
    items.removeOne(textItem);
    itemWidget->deleteLater();
    delete textItem;
    if (infoPanel)
        infoPanel->refresh();
}

int ImageViewer::itemsCount() { return items.size(); }

void ImageViewer::imageSelectionChanged() {
    if (imageView->scene()->selectedItems().empty())
        return;
    QList<TextItem *> items;
    for (QGraphicsItem *graphicsItem : imageView->scene()->selectedItems()) {
        QGraphicsProxyWidget *proxy = nullptr;
        TextItemWidget *itemWidget = nullptr;
        if ((proxy = dynamic_cast<QGraphicsProxyWidget *>(graphicsItem)) &&
            (itemWidget = dynamic_cast<TextItemWidget *>(proxy->widget()))) {
            int i;
            for (i = 0; i < items.size() && (
                    itemWidget->item()->pos().y() >= items[i]->pos().y() ||
                    itemWidget->item()->pos().x() > items[i]->pos().x()
            ); ++i);
            items.insert(i, itemWidget->item());
        }
    }
    if (items.empty())
        return;
    QList<QString> contents;
    while (!items.empty())
        contents.append(items.takeFirst()->text());
    QApplication::clipboard()->setText(contents.join(" "));
}