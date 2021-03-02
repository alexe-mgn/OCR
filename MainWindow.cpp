#include <QtCore/QByteArray>
#include <QtCore/QStringList>
#include <QtCore/QFileInfo>
#include <QtCore/QStandardPaths>
#include <QtCore/QMimeType>
#include <QtCore/QMimeDatabase>

#include <QtGui/QImage>
#include <QtGui/QImageReader>

#include <QtWidgets/QAction>
#include <QtWidgets/QDialog>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QFileDialog>

#include "Tabs/ImageViewer.h"
#include "MainWindow.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setupUi(this);

    // Tab bar
    tabBar = new QTabBar();
    centralLayout->insertWidget(centralLayout->indexOf(stackContainer), tabBar);
    tabBar->setExpanding(false);
    tabBar->setTabsClosable(true);
    tabBar->setMovable(true);
    connect(tabBar, &QTabBar::tabCloseRequested, this, &MainWindow::closeTab);
    connect(tabBar, &QTabBar::currentChanged, this, &MainWindow::setCurrentIndex);
    connect(tabBar, &QTabBar::tabMoved, this, &MainWindow::uiTabMoved);

    // Stack
    stack = new QStackedWidget();
    stackContainer->setCentralWidget(stack);

    // Dock panels
    panelDockWidgets = QList<PanelDockWidget *>{dockWidget,
                                                dockWidget_2,
                                                dockWidget_3};
    QList<PanelContainer *> containers{stackedWidget, stackedWidget_2, stackedWidget_3};
    auto dock = panelDockWidgets.begin();
    for (auto container = containers.begin(); dock != panelDockWidgets.end(); ++dock, ++container)
        (*dock)->setContainer(*container);

    // Home tab
    homeTab = new HomeTab();
    addTab(homeTab);
    //  tab
    tabBar->tabButton(0, QTabBar::RightSide)->deleteLater();
    tabBar->setTabButton(0, QTabBar::RightSide, nullptr);
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

void MainWindow::connectTab(Tab *tab) {
    tabBar->addTab(tab->windowTitle());
    stack->addWidget(tab);
    auto widgets = tab->getPanels();
    auto dock = panelDockWidgets.begin();
    for (auto i = widgets.begin(); i != widgets.end(); ++dock, ++i)
        if (*i)
            (*dock)->connectTab(tab, *i);
}

void MainWindow::addTab(Tab *tab) {
    connectTab(tab);
    stack->setCurrentWidget(tab);
    tabBar->setCurrentIndex(stack->currentIndex());
}

//bool MainWindow::loadFile(const QString &path) {
//    QImageReader reader(path);
//    reader.setAutoTransform(true);
//    QImage image = reader.read();
//    if (image.isNull()) {
//        QMessageBox msg(QMessageBox::Warning,
//                        tr("Input error"),
//                        tr("Cannot load %1").arg(QDir::toNativeSeparators(path)),
//                        QMessageBox::StandardButton::Abort,
//                        this);
//        msg.setDetailedText(reader.errorString());
//        msg.exec();
//        return false;
//    } else {
//        auto imageViewer = new ImageViewer();
//        imageViewer->setImage(image);
//        imageViewer->setWindowTitle(QFileInfo(path).fileName());
//        addTab(imageViewer);
//        return true;
//    }
//}

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
        if (auto imageViewer = ImageViewer::loadFile(dialog.selectedFiles().first())) {
            addTab(imageViewer);
            return;
        }
    }
}

void MainWindow::setCurrentIndex(int index) {
    tabBar->setCurrentIndex(index);
    stack->setCurrentIndex(index);
    for (auto &dock : panelDockWidgets)
        dock->setCurrentTab(dynamic_cast<Tab *>(stack->currentWidget()));
}

void MainWindow::closeTab(int index) {
    tabBar->removeTab(index);
    QWidget *w = stack->widget(index);
    stack->removeWidget(w);
    if (auto tab = dynamic_cast<Tab *>(w))
        for (auto &dock : panelDockWidgets)
            dock->removeTab(tab);
    w->deleteLater();
}

void MainWindow::moveTab(int from, int to) {
    tabBar->moveTab(from, to);
    QWidget *w = stack->widget(from);
    stack->removeWidget(w);
    stack->insertWidget(to, w);
}

void MainWindow::uiTabMoved(int from, int to) {
    QWidget *w = stack->widget(from);
    stack->removeWidget(w);
    stack->insertWidget(to, w);
}
