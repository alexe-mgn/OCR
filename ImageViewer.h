#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include "ImageView.h"
#include "Tab.h"


class ImageViewer : public Tab {
Q_OBJECT

public:
    explicit ImageViewer(QWidget *parent = nullptr);
    bool isSaveAvailable() override;

    ImageView *imageView;

    void setImage(QImage &newImage);
};


#endif
