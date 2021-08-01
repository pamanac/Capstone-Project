#include <QApplication>
#include "twoLevelTabWindow.h"

int ExampleTwoLevelTab(int argc, char* argv[]) {

    //QApplication app(argc, argv);

    twoLevelTabWindow window;
    window.show();

    //return app.exec();
    return 0;
}