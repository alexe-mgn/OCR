#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QtCore/QList>

#include <QtWidgets/QWidget>

#include "TextData.h"
#include "ToolPanels.h"
#include "ImageView.h"
#include "Tab.h"


class ImageViewer;

class IVInfoPanel : public InfoPanel {
Q_OBJECT

public:
    explicit IVInfoPanel(ImageViewer *viewerTab);

    void refresh() override;

protected:
    ImageViewer *imageViewer_;
};
////////////////////


class IVDataListPanel : public DataListPanel {
Q_OBJECT

public:
    explicit IVDataListPanel(ImageViewer *viewerTab);

protected:
    ImageViewer *imageViewer_;
};


////////////////////


class IVDataViewPanel : public DataViewPanel {
Q_OBJECT

public:
    explicit IVDataViewPanel(ImageViewer *viewerTab);

    void removeItem() override;

    void itemUpdated() override;

    void refreshRanges() override;

    TextItem *createItem() override;


private:
    ImageViewer *imageViewer_ = nullptr;
};


////////////////////


class ImageViewer : public Tab {
Q_OBJECT

public:
    static ImageViewer *loadFile(const QString &path);

    explicit ImageViewer(QWidget *parent = nullptr);

    ~ImageViewer() override;

    QList<QWidget *> getPanels() override;

    bool isSaveAvailable() override;

    void clear() override;

    bool isClearAvailable() override;

    bool hasImage();

    QRect imageRect();

    void setImage(const QImage &newImage);

    QString fileInfo();

    void setFileInfo(const QString &path);

    void addItem(TextItem *textItem);

    void refreshItem(TextItem *textItem);

    void removeItem(TextItem *textItem);

    int itemsCount();

protected:
    ImageView *imageView;
    QList<TextItem *> items_;

    IVInfoPanel *infoPanel;
    IVDataListPanel *dataListPanel;
    IVDataViewPanel *dataViewPanel;

    QString filePath;
};


#endif
