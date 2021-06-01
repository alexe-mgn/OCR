#ifndef RECOGNITION_H
#define RECOGNITION_H

#include <QtCore/QList>
#include <QtCore/QString>

#include <QtGui/QImage>

#include <random>

#include <TextData.h>


QString pyTracebackMessage();

class ImageCR {
public:
    static QString getPythonHome();

    static void init();

    static QList<TextItem *> scan(const QImage &image);

protected:
    static void finalize();

    static void *rec_f;
};


#endif //RECOGNITION_H
