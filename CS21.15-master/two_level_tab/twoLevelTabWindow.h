#pragma once

#include <QWidget>
#include <QTabBar>
#include <QStackedLayout>

class twoLevelTabWindow : public QWidget {
    Q_OBJECT
 
public:
    twoLevelTabWindow(QWidget* parent = 0);

private:
    QTabBar *tabs[2];
    QStackedLayout *frame;
    QVBoxLayout* v_layout;
};