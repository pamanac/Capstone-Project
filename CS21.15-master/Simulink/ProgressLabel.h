#ifndef SIMULINK_TEST_H
#define SIMULINK_TEST_H

#include <QTimer>
#include <QLabel>
#include <QVBoxLayout>

#include "windows.h"

class ProgressLabel : public QWidget {
private:
    QVBoxLayout* layout;

    QLabel* label;
    QTimer* timer;
    int progress;
    int volatile* progressPtr;

    DWORD dwThreadId;
    HANDLE hThread;

public:
    ProgressLabel(QWidget* parent = nullptr);
    void testProgress();
};

#endif