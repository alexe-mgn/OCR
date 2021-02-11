#include "ImageView.h"
#include "MainWindow.h"


int main(int argc, char **argv) {
    QApplication app (argc, argv);
    MainWindow mainWindow;
    mainWindow.show();
//    ImageViewer imageViewer;
//    imageViewer.show();
//    ImageView imageView;
//    imageView.show();
//    QLabel label;
//    label.setPixmap(QPixmap("C:\\Users\\admin\\Desktop\\котик.jpg", "jpg"));
//    label.show();
    return app.exec();
}