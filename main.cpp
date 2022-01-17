#include <QtWidgets/QApplication>
#include "MainWindow.h"

#include "Tabs/ImageViewer.h"


int main(int argc, char **argv) {
    QApplication app(argc, argv);
    MainWindow mainWindow;
    mainWindow.show();
    return QApplication::exec();
}