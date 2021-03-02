#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QtCore/QList>

#include <QtWidgets/QWidget>

#include "TextData.h"
#include "Panels/InfoPanel.h"
#include "Panels/DataListPanel.h"
#include "Panels/DataViewPanel.h"
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

public slots:

    virtual void selectItem(TextItem *textItem) {}

    TextItem *createItem() override;

protected:
    ImageViewer *imageViewer_;
};


////////////////////


class IVDataViewPanel : public DataViewPanel {
Q_OBJECT

public:
    explicit IVDataViewPanel(ImageViewer *viewerTab);

public slots:

    void refreshRanges() override;

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


    bool isClearAvailable() override;

    bool hasImage();

    QRect imageRect();

    QString fileInfo();

    int itemsCount();

public slots:

    void addItem(TextItem *textItem);

    void selectItem(TextItem *textItem);

    void refreshItem(TextItem *textItem);

    void removeItem(TextItem *textItem);

    void clear() override;

    void setImage(const QImage &newImage);

    void setFileInfo(const QString &path);

public:
    IVInfoPanel *infoPanel;
    IVDataListPanel *dataListPanel;
    IVDataViewPanel *dataViewPanel;
protected:
    ImageView *imageView;
    QList<TextItem *> items_;

    QString filePath;
};


#endif
