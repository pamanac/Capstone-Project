#ifndef EXAMPLE_BROWSER_H
#define EXAMPLE_BROWSER_H

#include <QtWidgets>
#include <QtCore>
#include "PrimaryWindow.h"

class ExampleBrowser : public QWidget {
	Q_OBJECT
public:
	ExampleBrowser();
	static QString returnStyleSheet(QString fname);
public slots: //Slots are registered by Qt for event handling.
	//void ExampleCustomLayout();
	//void ExampleModelViewBasic();
	//void ExampleModelViewIndex();
private:
	QPushButton* buttonCustomLayout;
	QPushButton* buttonModelViewBasic;
	QPushButton* buttonModelViewIndex;
	QPushButton* buttonExampleDataReading;
	QPushButton* buttonGearSim;
	QPushButton* buttonMatLab;
	QPushButton* buttonWireFrame;
	QPushButton* buttonJsonPager;

	QPushButton* buttonFilters2;
	QPushButton* buttonProgress;

	void GearSim();
	void wireFrame();
	void jsonPager();

	void filters2();
	void progress();
};

namespace ExamplesNamespace {
	int ExampleCustomLayout(int argc = 0, char* argv[] = nullptr);
	int ExampleModelViewBasic(int argc = 0, char* argv[] = nullptr);
	int ExampleModelViewIndex(int argc = 0, char* argv[] = nullptr);
	int ExampleDataReading(int argc = 0, char* argv[] = nullptr);
};

#endif