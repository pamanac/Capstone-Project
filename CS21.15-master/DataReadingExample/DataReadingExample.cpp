#pragma once
#include <QRegularExpression>
#include <QtCore>
#include <QtWidgets>
#include <QtCharts>
#include <QLayout>
#include "..\QtGearSim\ExampleBrowser.h"
#include "ExampleDataLineWidget.h"
#include "GearSimData.h"
#include "GearSimDataTable.h"
#include "GearSimDataFile.h"
#include <qtreeview.h>
#include "TreeModel.h"
#include "multiPager.h"

/*
Current file reading for just variable data:
* filename passed to readGearSimDatFile
* readGearSimDatFile invokes readDataFileLine to create labeledLineEditWidgets
* readGearSimDatFile returns a list of labeledLineEditWidget pointers for placement into a page.

suspected better method for all .dat data forms:
* filename passed to readGearSimDatFile
* readGearSimDatFile reads until variable section is encountered.  QFile object will be passed to the relevent function.
* variable data is read from a function.  Return list of labeledLineEditWidget pointer.
* graph data is read from a function.  Return QXYSeries.
*/

/*//TODO: PRUNE THIS FILE PLEASE
QTreeView* readFile(GearSimDataFile* file) {
	//Create GearSimFile

	//Create list
	QList<GearSimData*> variablesList;
	QList<labeledLineEditWidget*> returnList;

	//Fill that list
	variablesList = file->getVariablesList();
	//for (int i = 0; i < variablesList.length(); i++) {
	//	labeledLineEditWidget* newWidget = new labeledLineEditWidget(0, variablesList[i]->fullName, variablesList[i]->shortName, variablesList[i]->units);
	//	returnList.append(newWidget);
	//}
	//return returnList;
	return nullptr;
}

QList<tableWidget*> readTables(GearSimDataFile* file) {
	auto data = file->getDataTablesList();
	QList<tableWidget*> returnList;
	for (GearSimDataTable* table : data) {
		returnList.append(new tableWidget(0, table));
	}

	return returnList;
}
*/

//A helper function to show off the list from dat file reading.
void showDataFromFile(QString fileName, QGridLayout* layout) {
	GearSimDataFile* file = new GearSimDataFile(fileName);
	//QTreeView* dataFileContents = readFile(file);
	//QList<tableWidget*> tableFileContents = readTables(file);
	//int columnCount = 6;
	multiPager* pager = new multiPager(file);
	layout->addWidget(pager);
	//TreeModel* model = new TreeModel(file->getVariablesList());
	//QTreeView* view = new QTreeView;
	//view->setModel(model);
	//layout->addWidget(view);
	//for (int i = 0; !dataFileContents.isEmpty(); i++) {
	//	layout->addWidget(dataFileContents.takeFirst(), floor(i / columnCount), i % columnCount);
	//}
	//for (int i = 0; !tableFileContents.isEmpty(); i++) {
	//	layout->addWidget(tableFileContents.takeFirst(), floor(i / columnCount), i % columnCount);
	//}
}

int ExamplesNamespace::ExampleDataReading(int argc, char** argv) {
	//QApplication app(argc, argv);

	QGroupBox* data = new QGroupBox("Data group");
	QVBoxLayout* vertLayout = new QVBoxLayout;
	QGridLayout* dataLayout = new QGridLayout;
	labeledLineEditWidget* dataWindow_1 = new labeledLineEditWidget(nullptr, "TestText:", "Hello World", "");
	labeledLineEditWidget* dataWindow_2 = new labeledLineEditWidget(nullptr, "FileName:", "Enter a file to read", "");
	QPushButton* buttonReadDataFile = new QPushButton("Read File", nullptr);
	vertLayout->addWidget(dataWindow_1);
	vertLayout->addWidget(dataWindow_2);
	vertLayout->addWidget(buttonReadDataFile);
	vertLayout->addLayout(dataLayout);
	data->setLayout(vertLayout);
	
	//Just to help make it easier to read, we'll place things in a hbox.
	buttonReadDataFile->connect(buttonReadDataFile, &QPushButton::clicked, [=]() {showDataFromFile(dataWindow_2->dataLine->text(), dataLayout);});

	data->showMaximized();
	qDebug() << "ExampleDataReading Test Started";

	//return app.exec();
	return 0;
}