extern "C" {
#include "simulink.h"
}
#include <QTimer>
#include <QString>
#include <QLabel>

#include "ProgressLabel.h"
#include "windows.h"


ProgressLabel::ProgressLabel(QWidget* parent) : QWidget(parent) {
    layout = new QVBoxLayout();

    label = new QLabel();
    label->setStyleSheet("font-size: 80px");
    label->setText("0");
    timer = new QTimer(this);

    layout->addWidget(label);
    setLayout(layout);
    connect(timer, &QTimer::timeout, [=]() {
        DWORD dwExitCode = 0;
        LPDWORD lpdwExitCode = &dwExitCode;
        label->setText(QString::number(progress));
        if (GetExitCodeThread(hThread, lpdwExitCode) == 0) {
            timer->stop();
        }
    });
    progress = 0;
    progressPtr = &progress;

    dwThreadId = 0;
    hThread = nullptr;
}

void ProgressLabel::testProgress() {
    hThread = CreateThread(
        nullptr,                      // default security attributes
        0,                            // default stack size
        progressReport,               // function
        (LPVOID volatile)progressPtr, // param
        0,                            // default thread flags
        &dwThreadId                   // threadId
    );
    // show();
    timer->start(1000);
}