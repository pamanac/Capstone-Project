#include "twoLevelTabWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <qdebug.h>

twoLevelTabWindow::twoLevelTabWindow(QWidget* parent)
    : QWidget(parent) {

    tabs[0] = new QTabBar();
    tabs[0]->addTab("Configurations");
    tabs[0]->addTab("Simulations");
    tabs[0]->addTab("Renders");

    tabs[1] = new QTabBar();
    tabs[1]->addTab("1");
    tabs[1]->addTab("2");
    tabs[1]->addTab("3");

    frame = new QStackedLayout();
    QFont f("Arial", 20, QFont::Bold);
    for (int i = 1; i <= 9; i++) {
        QLabel *l = new QLabel(QString::number(i));
        l->setFont(f);
        int test = frame->addWidget(l);
    }

    v_layout = new QVBoxLayout(this);
    v_layout->addWidget(tabs[0]);
    v_layout->addWidget(tabs[1]);
    v_layout->addLayout(frame);

    connect(tabs[0], &QTabBar::currentChanged, [=](int i) {
        for (int j = tabs[1]->count(); j > 0; j--) {
            tabs[1]->removeTab(0);
        }
        for (int j = 0; j < 3; j++) {
            tabs[1]->addTab(QString::number(i * 3 + j + 1));
        }
    });

    connect(tabs[1], &QTabBar::currentChanged, [=](int i) {
        frame->setCurrentIndex(tabs[0]->currentIndex() * 3 + i);
    });
}