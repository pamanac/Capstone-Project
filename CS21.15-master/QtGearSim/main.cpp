#include <QApplication>
#include "ExampleBrowser.h"

// QString returnStyleSheet(QString fname);

int main(int argc, char* argv[]) {

    QApplication app(argc, argv); //governs user input and the event loop.
    app.setStyle(QStyleFactory::create("Fusion"));
    ExampleBrowser window;
    window.show();

    return app.exec(); //Invokes the event loop and hands over control.
}