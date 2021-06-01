#ifndef APP_H
#define APP_H

#include <QtWidgets/QApplication>


class App : QApplication {
public:
    App(int &argc, char **argv, int i = ApplicationFlags);

//    bool notify(QObject *object, QEvent *event) override;
};


#endif //APP_H
