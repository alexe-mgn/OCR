#include "HomeTab.h"


HomeTab::HomeTab(QWidget *parent) : Tab(parent) {
    setupUi(this);
}

bool HomeTab::isSaveAvailable() { return false; }