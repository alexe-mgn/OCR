#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H


#include <QtCore/QList>

#include <QtWidgets/QWidget>

#include "TextData.h"
#include "Tabs/ImageView.h"
#include "Tabs/ImageViewerPanels.h"
#include "Tabs/Tab.h"
#include "MainWindow.h"


class IVImageView : public ImageView {
Q_OBJECT

public:
    explicit IVImageView(ImageViewer *viewerTab);

protected:
    ImageViewer *imageViewer_ = nullptr;

    void mousePressEvent(QMouseEvent *event) override;
};


class ImageViewer : public Tab {
Q_OBJECT

public:
    static ImageViewer *loadImage(const QImage &image, MainWindow *mainWindow = nullptr);

    static ImageViewer *loadFile(const QString &path, MainWindow *mainWindow = nullptr);

    explicit ImageViewer(MainWindow *mainWindow = nullptr, QWidget *parent = nullptr);

    ~ImageViewer() override;

    QList<QWidget *> getPanels() override;

    bool isSaveAvailable() override;

    bool isExportAvailable() override;

    bool isClearAvailable() override;

    bool hasImage();

    QRect imageRect();

    QString fileInfo();

    int itemsCount();

public:
    TextItemWidget *widget(TextItem *textItem);

public slots:

    void addItem(TextItem *textItem);

    void selectItem(TextItem *textItem);

    void refreshItem(TextItem *textItem);

    void removeItem(TextItem *textItem);

    void clear() override;

    void setImage(const QImage &newImage);

    void setFileInfo(const QString &path);

    void imageSelectionChanged();

public:
    IVInfoPanel *infoPanel;
    IVDataListPanel *dataListPanel;
    IVDataViewPanel *dataViewPanel;
protected:
    IVImageView *imageView;
    QList<TextItem *> items;
    QList<TextItemWidget *> itemWidgets;

    QString filePath;
};


#endif //IMAGEVIEWER_H
