#ifndef RECOGNITION_H
#define RECOGNITION_H


#include <random>

#include <QtCore/QList>

#include <QtGui/QImage>

#include <TextData.h>


class ImageCR {
public:
    explicit ImageCR();

    void scan(const QImage &image);

    QList<TextItem *> items;
};


#endif //RECOGNITION_H
