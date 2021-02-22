#ifndef TAB_H
#define TAB_H

class Tab : public virtual QWidget {
public:
    explicit Tab(QWidget *parent = nullptr) : QWidget(parent) {}

    virtual bool isSaveAvailable() = 0;
};


#endif
