#ifndef IMAGEVIEWERPANELS_H
#define IMAGEVIEWERPANELS_H


#include "TextData.h"
#include "Panels/InfoPanel.h"
#include "Panels/DataListPanel.h"
#include "Panels/DataViewPanel.h"


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


#endif //IMAGEVIEWERPANELS_H
