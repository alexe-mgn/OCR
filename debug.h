#ifndef DEBUG_H
#define DEBUG_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include <QtCore/QPoint>

namespace debug {
    class PointerWidget : public QWidget {
    Q_OBJECT
    public:

        explicit PointerWidget(QWidget *parent = nullptr);

        void move(const QPoint &pos);

        void move(int x, int y) {move(QPoint(x, y));}

    private:
        QLabel *label;
    };

    std::string getEventTypeName(unsigned int type);
}

#endif
