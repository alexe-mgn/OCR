#include <QtWidgets/QApplication>
#include "MainWindow.h"

#include "Tabs/ImageViewer.h"
int main(int argc, char **argv) {
    QApplication app(argc, argv);
    MainWindow mainWindow;
    mainWindow.addTab(ImageViewer::loadFile("D:/HSE/AaPC/OCR/recognition/wiki.jpg", &mainWindow));
    mainWindow.addTab(ImageViewer::loadFile("D:/HSE/AaPC/OCR/recognition/wiki_p.jpg", &mainWindow));
    mainWindow.addTab(ImageViewer::loadFile("D:/HSE/AaPC/OCR/recognition/wiki_small.jpg", &mainWindow));
    mainWindow.show();
    return QApplication::exec();
}