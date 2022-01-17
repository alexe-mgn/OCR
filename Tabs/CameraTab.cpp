#include <QtCore/QSignalBlocker>

#include <QtWidgets/QComboBox>

#include "ImageViewer.h"
#include "CameraTab.h"


CameraPanel::CameraPanel(CameraTab *cameraTab) : QWidget() {
    setupUi(this);

    cameraTab_ = cameraTab;
#ifndef QT_OLD_CAMERA
    connect(new QMediaDevices(), &QMediaDevices::videoInputsChanged,
            this, &CameraPanel::updateDevices);
#endif
    connect(deviceValue, &QComboBox::currentTextChanged,
            cameraTab_,
            static_cast<void (CameraTab::*)(
                    const QString &)>(&CameraTab::setCamera));
    connect(captureButton, &QPushButton::clicked, cameraTab_,
            &CameraTab::capture);
    // TODO save button
    saveButton->deleteLater();
}

void CameraPanel::updateDevices() {
    const QSignalBlocker blocker(deviceValue);
    QCamera *camera = cameraTab_->camera();
    deviceValue->clear();
    deviceValue->addItem(QString(""));
#ifdef QT_OLD_CAMERA
    for (const QCameraInfo &ci: QCameraInfo::availableCameras()) {
        deviceValue->addItem(ci.deviceName());
    }
    QString deviceName = camera ? QCameraInfo(*camera).deviceName() : "";
    if (deviceValue->currentText() != deviceName) {
        cameraTab_->setCamera(nullptr);
    }
#else
    for (const QCameraDevice &cd: QMediaDevices::videoInputs()) {
        deviceValue->addItem(cd.description());
    }
    QString deviceName = camera ? camera->cameraDevice().description() : "";
    if (deviceValue->currentText() != deviceName) {
        cameraTab_->setCamera(nullptr);
    }
#endif
    updateState();
}

void CameraPanel::updateState() {
    QCamera *camera = cameraTab_->camera();
#ifdef QT_OLD_CAMERA
    bool available = camera && camera->isAvailable() &&
                     camera->state() == QCamera::ActiveState;
#else
    bool available = camera && camera->isAvailable();
#endif
    captureButton->setEnabled(available);
    // TODO save button
//    saveButton->setEnabled(available);
}


////////////////////


CameraTab::CameraTab(MainWindow *mainWindow, QWidget *parent) : Tab(mainWindow,
                                                                    parent) {
    setWindowTitle(tr("Camera"));

    viewFinder = new QVideoWidget(this);
    setLayout(new QVBoxLayout());
    layout()->addWidget(viewFinder);

    cameraPanel = new CameraPanel(this);
    cameraPanel->updateDevices();
}

CameraTab::~CameraTab() {
    if (camera_) {
        camera_->stop();
        delete camera_;
        delete imageCapture_;
    }
}

QList<QWidget *> CameraTab::getPanels() {
    return QList<QWidget *>{cameraPanel};
}

//bool CameraTab::isSaveAvailable() { return camera_ && camera_->isAvailable(); }

QCamera *CameraTab::camera() {
    return camera_;
}

void CameraTab::setCamera(QCamera *camera) {
    if (camera == camera_)
        return;
    if (camera_) {
        if (camera_->isAvailable())
            camera_->stop();
#ifndef QT_OLD_CAMERA
        delete captureSession_;
#endif
        delete imageCapture_;
        delete camera_;
    }
    camera_ = camera;
    if (camera_) {
#ifdef QT_OLD_CAMERA
        imageCapture_ = new ImageCapture(camera);
        cameraPanel->deviceValue->setCurrentText(
                QCameraInfo(*camera_).deviceName());
#else
        captureSession_ = new QMediaCaptureSession();
        imageCapture_ = new ImageCapture();
        captureSession_->setImageCapture(imageCapture_);
        captureSession_->setCamera(camera_);
        captureSession_->setVideoOutput(viewFinder);
        cameraPanel->deviceValue->setCurrentText(
                camera_->cameraDevice().description());
#endif
    } else
        cameraPanel->deviceValue->setCurrentText("");
    initCamera();
}

void CameraTab::setCamera(const QString &deviceName) {
    if (!deviceName.isEmpty()) {
#ifdef QT_OLD_CAMERA
        setCamera(new QCamera(deviceName.toUtf8()));
        if (camera_ == nullptr)
            cameraPanel->updateDevices();
#else
        for (const QCameraDevice &cd: QMediaDevices::videoInputs())
            if (cd.description() == deviceName) {
                setCamera(new QCamera(cd));
                break;
            }
        if (camera_ == nullptr || camera_->cameraDevice().description() != deviceName) {
            cameraPanel->updateDevices();
        }
#endif
    } else
        setCamera(nullptr);
}

void CameraTab::initCamera() {
    if (camera_) {
#ifdef QT_OLD_CAMERA
        imageCapture_->setCaptureDestination(
                QCameraImageCapture::CaptureToBuffer);
        connect(camera_, &QCamera::stateChanged, this, &CameraTab::updateState);
#endif
        connect(imageCapture_, &ImageCapture::imageCaptured, this,
                &CameraTab::imageCaptured);
#ifdef QT_OLD_CAMERA
        camera_->setViewfinder(viewFinder);
#endif
        camera_->start();
    }
    updateState();
}

void CameraTab::updateState() {
    cameraPanel->updateState();
}

void CameraTab::capture() {
    updateState();
    imageCapture_->capture();
}

void CameraTab::imageCaptured(int id, const QImage &image) {
    ImageViewer *tab = ImageViewer::loadImage(image, main);
    tab->setFileInfo("camera_image.jpg");
    main->addTab(tab);
}
