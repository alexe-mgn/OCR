#include <QtCore/QMap>
#include <QtCore/QSignalBlocker>
#include <QtCore/QTextStream>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>

#include <QtGui/QClipboard>
#include <QtGui/QImageReader>

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QFileDialog>

#include "recognition.h"
#include "ImageViewer.h"
#include "TextData.h"


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


ImageViewer *ImageViewer::loadImage(const QImage &image, MainWindow *mainWindow) {
    auto imageViewer = new ImageViewer(mainWindow);
    imageViewer->setImage(image);
    return imageViewer;
}


ImageViewer *ImageViewer::loadFile(const QString &path, MainWindow *mainWindow) {
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
        auto imageViewer = ImageViewer::loadImage(image, mainWindow);
        imageViewer->setFileInfo(path);
//        imageViewer->setWindowTitle(QFileInfo(path).fileName());
        return imageViewer;
    }
}


ImageViewer::ImageViewer(MainWindow *mainWindow, QWidget *parent) : Tab(mainWindow, parent) {
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
    for (auto w : QList<QWidget *>{infoPanel, dataListPanel, dataViewPanel})
        w->deleteLater();
}

QList<QWidget *> ImageViewer::getPanels() {
    return QList<QWidget *>{infoPanel, dataListPanel, dataViewPanel};
}

//bool ImageViewer::isSaveAvailable() { return imageView && imageView->isSaveAvailable(); }

void ImageViewer::exportData() {
    QFileDialog dialog(this, tr("Choose output file"));
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setAcceptMode(QFileDialog::AcceptSave);

    QMap<QString, QString (*)(const QList<TextItem *> &)> filters = {
            {"CSV (*.csv)", &itemsToCSV},
            {"TXT (*.txt)", &itemsToText}
    };
    dialog.setNameFilters(filters.keys());

    dialog.setDirectory(exportPath);

    while (dialog.exec() == QDialog::Accepted) {
        exportPath = dialog.directory();
        QFile file(dialog.selectedFiles().first());
        if (file.open(QFile::WriteOnly | QFile::Text)) {
            file.write(filters[dialog.selectedNameFilter()](items).toUtf8());
            file.close();
            return;
        }
    }
}

bool ImageViewer::isExportAvailable() { return !items.empty(); }

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
    connect(widget, &TextItemWidget::clicked, [this, widget] { this->selectItem(widget->item()); });
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

void ImageViewer::scan() {
    if (hasImage()) {
        ImageCR cr;
        cr.scan(imageView->image());
        for (TextItem *item : cr.items)
            addItem(item);
    }
}

void ImageViewer::imageSelectionChanged() {
    if (imageView->scene()->selectedItems().empty())
        return;
    QList<TextItem *> selectedItems;
    for (QGraphicsItem *graphicsItem : imageView->scene()->selectedItems()) {
        QGraphicsProxyWidget *proxy;
        TextItemWidget *itemWidget;
        if ((proxy = dynamic_cast<QGraphicsProxyWidget *>(graphicsItem)) &&
            (itemWidget = dynamic_cast<TextItemWidget *>(proxy->widget()))) {
            selectedItems.append(itemWidget->item());
        }
    }
    if (!selectedItems.empty())
        QApplication::clipboard()->setText(itemsToText(selectedItems));
}