#include <QtCore/QSignalBlocker>

#include "ImageViewer.h"
#include "CameraTab.h"


CameraPanel::CameraPanel(CameraTab *cameraTab) : QWidget() {
    setupUi(this);

    cameraTab_ = cameraTab;
    connect(deviceValue, &QComboBox::currentTextChanged,
            cameraTab_,
            static_cast<void (CameraTab::*)(const QString &)>(&CameraTab::setCamera));
    connect(captureButton, &QPushButton::clicked, cameraTab_, &CameraTab::capture);
    // TODO save button
    saveButton->deleteLater();
}

void CameraPanel::updateDevices() {
    const QSignalBlocker blocker(deviceValue);
    QCamera *camera = cameraTab_->camera();
    deviceValue->clear();
    deviceValue->addItem(QString(""));
    for (const QCameraInfo &ci : QCameraInfo::availableCameras()) {
        deviceValue->addItem(ci.deviceName());
    }
    QString deviceName = camera ? QCameraInfo(*camera).deviceName() : "";
    if (deviceValue->currentText() != deviceName) {
        cameraTab_->setCamera(nullptr);
    }
    deviceValue->setCurrentText(deviceName);
    updateState();
}

void CameraPanel::updateState() {
    QCamera *camera = cameraTab_->camera();
    bool available = camera && camera->isAvailable() && camera->state() == QCamera::ActiveState;
    captureButton->setEnabled(available);
    // TODO save button
//    saveButton->setEnabled(available);
}


////////////////////


CameraTab::CameraTab(MainWindow *mainWindow, QWidget *parent) : Tab(mainWindow, parent) {
    setWindowTitle(tr("Camera"));

    viewFinder = new QCameraViewfinder(this);
    setLayout(new QVBoxLayout());
    layout()->addWidget(viewFinder);

    cameraPanel = new CameraPanel(this);
    cameraPanel->updateDevices();
}

CameraTab::~CameraTab() {
    cameraPanel->deleteLater();
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
    if (camera_) {
        camera_->stop();
        delete camera_;
        delete imageCapture_;
    }
    camera_ = camera;
    if (camera_)
        imageCapture_ = new QCameraImageCapture(camera);
    initCamera();
}

void CameraTab::setCamera(const QString &deviceName) {
    setCamera(
            deviceName.isEmpty() ? nullptr : new QCamera(deviceName.toUtf8())
    );
}

void CameraTab::initCamera() {
    if (camera_) {
        imageCapture_->setCaptureDestination(QCameraImageCapture::CaptureToBuffer);
        connect(camera_, &QCamera::stateChanged, this, &CameraTab::updateState);
        connect(imageCapture_, &QCameraImageCapture::imageCaptured, this, &CameraTab::imageCaptured);
        camera_->setViewfinder(viewFinder);
        camera_->start();
    }
}

void CameraTab::updateState() {
    cameraPanel->updateState();
}

void CameraTab::capture() {
    imageCapture_->capture();
}

void CameraTab::imageCaptured(int id, const QImage &image) {
    ImageViewer *tab = ImageViewer::loadImage(image, main);
    tab->setFileInfo("camera_image.jpg");
    main->addTab(tab);
}
