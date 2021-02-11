#include "ImageView.h"
#include "MainWindow.h"

#include <QtCore/QStandardPaths>
#include <QtCore/QMimeType>
#include <QtCore/QMimeDatabase>
#include <QtGui/QImageReader>
#include <QtWidgets/QTabBar>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QFileDialog>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setupUi(this);

    // tabBar
    tabBar = new QTabBar();
    centralWidget()->layout()->addWidget(tabBar);
    tabBar->setTabsClosable(true);
    tabBar->setMovable(true);
    connect(tabBar, &QTabBar::tabCloseRequested, this, &MainWindow::closeTab);
    connect(tabBar, &QTabBar::currentChanged, this, &MainWindow::changeTab);
    connect(tabBar, &QTabBar::tabMoved, this, &MainWindow::moveTab);

    // stackedWidget
    stackedWidget = new QStackedWidget();
    centralWidget()->layout()->addWidget(stackedWidget);

    // homeTab
    homeTab = new HomeTab();
    //  tab
    tabBar->addTab("Home");
    tabBar->tabButton(0, QTabBar::RightSide)->deleteLater();
    tabBar->setTabButton(0, QTabBar::RightSide, nullptr);
    //  stack
    stackedWidget->addWidget(homeTab);
    //  connections
    connect(homeTab->openButton, &QPushButton::clicked, actionOpen, &QAction::trigger);
    connect(homeTab->exitButton, &QPushButton::clicked, actionExit, &QAction::trigger);

    // actions
    connect(actionExit, &QAction::triggered, this, &MainWindow::close);
    connect(actionOpen, &QAction::triggered, this, &MainWindow::chooseFile);

    const QStringList sp = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
    fileLastPath = sp.empty() ? QDir::current() : sp.last();

    for (QMainWindow *&window : this->findChildren<QMainWindow *>()) {
        window->setWindowFlags(Qt::Widget);
    }
}

void MainWindow::connectTab(ImageView *tab) {
    tabBar->addTab(tab->windowTitle());
    stackedWidget->addWidget(tab);
}

void MainWindow::addTab(ImageView *tab) {
    connectTab(tab);
    stackedWidget->setCurrentWidget(tab);
    tabBar->setCurrentIndex(stackedWidget->currentIndex());
}

bool MainWindow::loadFile(QString &path) {
    QImageReader reader(path);
    reader.setAutoTransform(true);
    QImage image = reader.read();
    if (image.isNull()) {
        QMessageBox msg(QMessageBox::Warning,
                        tr("Input error"),
                        tr("Cannot load %1").arg(QDir::toNativeSeparators(path)),
                        QMessageBox::StandardButton::Abort,
                        this);
        msg.setDetailedText(reader.errorString());
        msg.exec();
        return false;
    } else {
        auto imageView = new ImageView();
        imageView->setImage(image);
        imageView->setWindowTitle(QFileInfo(path).fileName());
        addTab(imageView);
        return true;
    }
}

void MainWindow::chooseFile() {
    QFileDialog dialog(this, tr("Choose image"));
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);

    QMimeDatabase mimeDB;
    QStringList mimeFilters;
    QStringList nameFilter;
    for (const QByteArray &mimeTypeFilter : QImageReader::supportedMimeTypes()) {
        QMimeType mimeType = mimeDB.mimeTypeForName(mimeTypeFilter);
        if (mimeType.isValid()) {
            mimeFilters.append(mimeTypeFilter);
            nameFilter.append(mimeType.globPatterns());
        }
    }

    mimeFilters.prepend("application/octet-stream");
    dialog.setMimeTypeFilters(mimeFilters);

    QStringList nameFilters = dialog.nameFilters();
    nameFilters.prepend(tr("Image (%1)").arg(nameFilter.join(' ')));
    dialog.setNameFilters(nameFilters);
    dialog.selectNameFilter(nameFilters.at(0));

    dialog.setDirectory(fileLastPath);

    while (dialog.exec() == QDialog::Accepted) {
        fileLastPath = dialog.directory();
        if (loadFile(dialog.selectedFiles().first()))
            return;
    }
}

void MainWindow::changeTab(int index) {
    stackedWidget->setCurrentIndex(index);
}

void MainWindow::closeTab(int index) {
    tabBar->removeTab(index);
    QWidget *w = stackedWidget->widget(index);
    stackedWidget->removeWidget(w);
    w->deleteLater();
}

void MainWindow::moveTab(int from, int to) {
    QWidget *w = stackedWidget->widget(from);
    stackedWidget->removeWidget(w);
    stackedWidget->insertWidget(to, w);
}
