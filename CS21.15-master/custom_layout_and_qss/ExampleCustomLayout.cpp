#pragma once
#include <QtCore>
#include <QtWidgets>
#include "window.h"
#include "..\QtGearSim\ExampleBrowser.h"

int ExamplesNamespace::ExampleCustomLayout(int argc, char** argv) { //normally, this returns an int
	//QApplication app(argc, argv);

	MyWindow win; //As seen in window.h
	win.resize(800, 600);
	win.setVisible(true);
	win.show();
	qDebug() << "STARTING TEST";

	//return app.exec();
	return 0;
}