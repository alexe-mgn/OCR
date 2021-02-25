#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QtCore/QList>

#include <QtWidgets/QWidget>

#include "ImageView.h"
#include "Tab.h"
#include "ui_InfoPanel.h"
#include "ui_DataListPanel.h"


class InfoPanel : public QWidget, public Ui::InfoPanel {
public:
    explicit InfoPanel(QWidget *parent = nullptr) : QWidget(parent) { setupUi(this); }
};

class DataListPanel : public QWidget, public Ui::DataListPanel {
public:
    explicit DataListPanel(QWidget *parent = nullptr) : QWidget(parent) { setupUi(this); }
};

class ImageViewer : public Tab {
Q_OBJECT

public:
    explicit ImageViewer(QWidget *parent = nullptr);

    ~ImageViewer() override;

    QList<QWidget *> getPanels() override;

    bool isSaveAvailable() override;


    void setImage(QImage &newImage);

protected:
    ImageView *imageView;
    InfoPanel *infoPanel;
    DataListPanel *dataListPanel;
    QWidget *dataViewPanel;
};


#endif
