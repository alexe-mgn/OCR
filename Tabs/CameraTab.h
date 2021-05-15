#ifndef CAMERATAB_H
#define CAMERATAB_H


#include <QtCore/QList>

#include <QtGui/QImage>

#include <QtWidgets/QWidget>
#include <QtWidgets/QVBoxLayout>

//#include <QtMultimedia/QVideoFrame>
#include <QtMultimedia/QCameraInfo>
#include <QtMultimedia/QCamera>
#include <QtMultimedia/QCameraImageCapture>
#include <QtMultimediaWidgets/QCameraViewfinder>

#include "ui_CameraPanel.h"
#include "Tabs/Tab.h"
#include "MainWindow.h"


class CameraTab;

class CameraPanel : public QWidget, public Ui::CameraPanel {
Q_OBJECT

public:
    explicit CameraPanel(CameraTab *cameraTab);

public slots:
    virtual void updateDevices();

    virtual void updateState();

protected:
    CameraTab *cameraTab_;
};


////////////////////


class CameraTab : public Tab {
Q_OBJECT

public:
    explicit CameraTab(MainWindow *mainWindow = nullptr, QWidget *parent = nullptr);

    ~CameraTab() override;

    QList<QWidget *> getPanels() override;

//    bool isSaveAvailable() override;

    virtual QCamera *camera();

    virtual void setCamera(QCamera *camera);

    virtual void setCamera(const QString &deviceName);

public slots:
    virtual void initCamera();

    virtual void updateState();

    virtual void capture();

public:
    CameraPanel *cameraPanel;

protected:
    QString filePath;
    QCamera *camera_ = nullptr;
    QCameraImageCapture *imageCapture_ = nullptr;
    QCameraViewfinder *viewFinder;

protected slots:
    virtual void imageCaptured(int id, const QImage &image);
};


#endif //CAMERATAB_H
