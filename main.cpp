#include "MainWindow.h"
#include "ImageViewer.h"

int main(int argc, char **argv) {
    QApplication app (argc, argv);
    MainWindow mainWindow;
    mainWindow.show();
//    ImageViewer imageViewer;
//    imageViewer.show();
    return app.exec();
}