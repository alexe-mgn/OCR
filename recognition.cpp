#include <cmath>
#include <algorithm>
#include <ctime>
#include <random>

#include <QtCore/QRect>

#include "recognition.h"


ImageCR::ImageCR() = default;

void ImageCR::scan(const QImage &image) {
    std::default_random_engine gen(std::time(nullptr));
    std::uniform_int_distribution dx(0, image.size().width());
    std::uniform_int_distribution dy(0, image.size().height());
    std::lognormal_distribution sdx(std::log((float) image.size().width() / 30),
                                    std::log((float) image.size().width() / 300));
    std::lognormal_distribution sdy(std::log((float) image.size().height() / 40),
                                    std::log((float) image.size().height() / 1200));
    std::lognormal_distribution sdln(std::log(10));

    const char charset[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";
    const int chars = sizeof(charset);

    std::uniform_int_distribution sdch(0, chars - 1);

    int n = 15;
    for (int i = 0; i < n; ++i) {
        int w = std::clamp((int) sdx(gen), 20, image.size().width());
        int h = std::clamp((int) sdy(gen), 10, image.size().height());
        int x = std::clamp(dx(gen), w / 2, image.size().width() - w / 2);
        int y = std::clamp(dy(gen), h / 2, image.size().height() - h / 2);
        int ln = (int) sdln(gen);
        char text[ln];
        for (int j = 0; j < ln; text[j++] = charset[sdch(gen)]);
        items.append(new TextItem(QRect(x - w / 2, y - h / 2, w, h), text));
    }
}