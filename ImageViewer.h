#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include "ui_ImageViewer.h"

#include <QtWidgets/QMessageBox>
#include <QtWidgets/QFileDialog>
#include <QtGui/QScreen>
#include <QtGui/QImageReader>
#include <QtGui/QImageWriter>
#include <QtCore/QString>
#include <QtCore/QDir>
#include <QtCore/QStandardPaths>

class ImageViewer : public QMainWindow, public Ui::ImageViewer {
Q_OBJECT

public:
    explicit ImageViewer(QWidget *parent = nullptr);

    void connectActions();

    void updateActions();

    void setImage(QImage &newImage);

    void setScale(double scale = 1.0);

    void zoomIn();

    void zoomOut();

    void resetScale();

    void fitToWindow();

    void loadImage(QImage &newImage);

    bool loadFile(QString &path);

    void chooseFile();

public:
    QImage image;
    double imageScale;
    QString imagePath;
    QDir lastDirectory;
};

#endif
