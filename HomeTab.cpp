#include "HomeTab.h"


HomeTab::HomeTab(QWidget *parent) : Tab(parent) {
    setupUi(this);
}

QList<QWidget *> HomeTab::getPanels() {
    return QList<QWidget *>();
}

bool HomeTab::isSaveAvailable() { return false; }
