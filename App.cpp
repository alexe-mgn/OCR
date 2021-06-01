#include <exception>

#include "App.h"

App::App(int &argc, char **argv, int i) : QApplication(argc, argv, i) {}

// TODO exception handler
//bool App::notify(QObject *object, QEvent *event) {
//    bool res = true;
//    try {
//        res = QApplication::notify(object, event);
//    } catch (const std::exception& ex) {
//
//    }
//    return res;
//}
