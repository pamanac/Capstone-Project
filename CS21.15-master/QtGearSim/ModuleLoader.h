#ifndef MODULE_LOADER_H
#define MODULE_LOADER_H

#include <QJsonObject>
#include <QJsonValue>
#include <QList>
#include <QString>

#include "MainWindow.h"
#include "QJsonModel.h"

// TODO: ModuleLoader really should check for cycles to avoid infinite loops
// here is some excellent reference: https://stackoverflow.com/questions/261573/best-algorithm-for-detecting-cycles-in-a-directed-graph
// at first thought, i think loadObject and loadArray can keep an additional
// parameter storing the key chain down from the root, and if the current key
// is in that key chain, then that means there is a cyclical definition and
// the program should refuse the modules.json file
class ModuleLoader {
public:
	MainWindow::ProgramState::UserSelections userSelections;
	QJsonDocument modulesJdoc;
	QJsonObject modulesObj;
	QJsonModel* model;

	ModuleLoader() = default;
	ModuleLoader(MainWindow::ProgramState::UserSelections userSelections);

	QJsonObject loadRoot(QList<QString> keys);
	QJsonObject loadObject(QString key, QJsonObject parentObj);
	QJsonValue loadArray(QString moduleKey, QJsonValue value);
	//TODO: add method to load a simulation JSON file
	//TODO: add method to load an aircraft JSON file
	//TODO: add method to combine both aforementioned JSON files.
};

#endif