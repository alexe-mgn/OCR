#ifndef CAMERATAB_H
#define CAMERATAB_H

#include <QtGlobal>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#define QT_OLD_CAMERA
#endif

#include <QtCore/QList>

#include <QtGui/QImage>

#include <QtWidgets/QWidget>
#include <QtWidgets/QVBoxLayout>

#include <QtMultimedia/QCamera>
#include <QtMultimediaWidgets/QVideoWidget>

#ifdef QT_OLD_CAMERA

#include <QtMultimedia/QCameraInfo>
#include <QtMultimedia/QCameraImageCapture>
using ImageCapture = QCameraImageCapture;

#else

#include <QtMultimedia/QMediaDevices>
#include <QtMultimedia/QCameraDevice>
#include <QtMultimedia/QImageCapture>
#include <QtMultimedia/QMediaCaptureSession>
using ImageCapture = QImageCapture;

#endif

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
    explicit CameraTab(MainWindow *mainWindow = nullptr,
                       QWidget *parent = nullptr);

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
    ImageCapture *imageCapture_ = nullptr;
    QVideoWidget *viewFinder;
#ifndef QT_OLD_CAMERA
    QMediaCaptureSession *captureSession_ = nullptr;
#endif

protected slots:

    virtual void imageCaptured(int id, const QImage &image);
};


#endif //CAMERATAB_H
