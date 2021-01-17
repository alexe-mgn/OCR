#include "MainWindow.h"
#include <QtWidgets/QTabBar>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setupUi(this);

    const QStringList sp = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
    fileLastPath = sp.empty() ? QDir::current() : sp.last();

    for (QMainWindow *&window : this->findChildren<QMainWindow *>()) {
        window->setWindowFlags(Qt::Widget);
    }

    tabWidget->setTabsClosable(true);
    tabWidget->tabBar()->tabButton(0, QTabBar::RightSide)->deleteLater();
    tabWidget->tabBar()->setTabButton(0, QTabBar::RightSide, nullptr);;
}

void MainWindow::connectTab(ImageViewer *tab) {
    disconnect(tab->actionOpen, &QAction::triggered, tab, &ImageViewer::chooseFile);
    connect(tab->actionOpen, &QAction::triggered, actionOpen, &QAction::trigger);
    tab->setWindowFlag(Qt::Window, false);
    tab->setWindowFlag(Qt::Widget, true);
    tabWidget->addTab(tab, tab->windowTitle());
}

void MainWindow::chooseFile() {
    QFileDialog dialog(this, tr("Choose image"));
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);

    QStringList mimeTypeFilters;
    for (const QByteArray &mimeType : QImageReader::supportedMimeTypes())
        mimeTypeFilters.append(mimeType);
    mimeTypeFilters.sort();
    mimeTypeFilters.prepend("application/octet-stream");
    dialog.setMimeTypeFilters(mimeTypeFilters);
    dialog.selectMimeTypeFilter("application/octet-stream");

    dialog.setDirectory(fileLastPath);

    auto tab = new ImageViewer();
    tab->setVisible(false);
    while (dialog.exec() == QDialog::Accepted) {
        fileLastPath = dialog.directory();
        if (tab->loadFile(dialog.selectedFiles().first())) {
            connectTab(tab);
            return;
        }
    }
    tab->deleteLater();
}

void MainWindow::on_tabWidget_tabCloseRequested(int index) {
    tabWidget->removeTab(index);
}

void MainWindow::on_actionOpen_triggered() {
    chooseFile();
}

void MainWindow::on_actionExit_triggered() {
    close();
}