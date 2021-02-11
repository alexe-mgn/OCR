#include "ImageViewer.h"


ImageViewer::ImageViewer(QWidget *parent) : QMainWindow(parent) {
    setupUi(this);
    imageScale = 1.0;

    const QStringList sp = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
    lastDirectory = sp.empty() ? QDir::current() : sp.last();

    scrollArea->setStyleSheet("border: 2px solid green;");

    imageLabel->setScaledContents(false);
    imageLabel->setStyleSheet("border: 2px solid red;");

    connectActions();
    updateActions();

    setWindowTitle("");
}

void ImageViewer::connectActions() {
    connect(actionOpen, &QAction::triggered, this, &ImageViewer::chooseFile);
    connect(actionZoomIn, &QAction::triggered, this, &ImageViewer::zoomIn);
    connect(actionZoomOut, &QAction::triggered, this, &ImageViewer::zoomOut);
    connect(actionResetSize, &QAction::triggered, this, &ImageViewer::resetScale);
    connect(actionFitToWindow, &QAction::triggered, this, &ImageViewer::fitToWindow);
}

void ImageViewer::updateActions() {
    actionZoomIn->setEnabled(!image.isNull() && !actionFitToWindow->isChecked());
    actionZoomOut->setEnabled(!image.isNull() && !actionFitToWindow->isChecked());
    actionResetSize->setEnabled(!image.isNull() && !actionFitToWindow->isChecked());
    actionFitToWindow->setEnabled(!image.isNull());
}

void ImageViewer::setImage(QImage &newImage) {
    image = newImage;
    imageLabel->setPixmap(QPixmap::fromImage(image));
}

void ImageViewer::setScale(double scale) {
    imageScale = scale;
    imageLabel->resize(scale * imageLabel->pixmap()->size());
}

void ImageViewer::zoomIn() {
    setScale(imageScale * 1.25);
}

void ImageViewer::zoomOut() {
    setScale(imageScale * 0.8);
}

void ImageViewer::resetScale() {
    setScale();
}

void ImageViewer::fitToWindow() {
    scrollArea->setWidgetResizable(actionFitToWindow->isChecked());
    if (!actionFitToWindow->isChecked())
        imageLabel->adjustSize();
    updateActions();
}

void ImageViewer::loadImage(QImage &newImage) {
    setImage(newImage);
    resetScale();
    fitToWindow();
    updateActions();
}

bool ImageViewer::loadFile(QString &path) {
    QImageReader reader(path);
    reader.setAutoTransform(true);
    QImage newImage = reader.read();
    if (newImage.isNull()) {
        QMessageBox msg(QMessageBox::Warning,
                        tr("Input error"),
                        tr("Cannot load %1").arg(QDir::toNativeSeparators(path)),
                        QMessageBox::StandardButton::Abort,
                        this);
        msg.setDetailedText(reader.errorString());
        msg.exec();
        return false;
    } else {
        loadImage(newImage);
        imagePath = path;
        setWindowTitle(QFileInfo(imagePath).fileName());
        return true;
    }
}

void ImageViewer::chooseFile() {
    QFileDialog dialog(this, tr("Choose image"));
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);

    QStringList mimeTypeFilters;
    for (const QByteArray &mimeType : QImageReader::supportedMimeTypes())
        mimeTypeFilters.append(mimeType);
    mimeTypeFilters.sort();
    mimeTypeFilters.prepend("application/octet-stream");
    dialog.setMimeTypeFilters(mimeTypeFilters);
    dialog.selectMimeTypeFilter("application/octet-stream");

    dialog.setDirectory(lastDirectory);

    while (dialog.exec() == QDialog::Accepted) {
        lastDirectory = dialog.directory();
        if (loadFile(dialog.selectedFiles().first())) return;
    }
}
