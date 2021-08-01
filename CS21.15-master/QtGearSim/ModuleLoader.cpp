#include <QFile>

#include "ModuleLoader.h"
#include "MainWindow.h"

ModuleLoader::ModuleLoader(MainWindow::ProgramState::UserSelections userSelections) : userSelections(userSelections) {
	QFile file(":/data/Resources/modules.json");
	file.open(QIODevice::ReadOnly | QIODevice::Text);
	QByteArray data = file.readAll();
	file.close();

	modulesJdoc = QJsonDocument::fromJson(data);
	modulesObj = modulesJdoc.object();

	model = new QJsonModel();
	QJsonObject rootObj = loadRoot({ userSelections.airplane, userSelections.simulation });

	model->loadItem(rootObj, "root", nullptr);
}

// loadRoot treats primitive root items differently, so that the structure will be
// DropAircraft / DropAircraft / ACMass, for example, for primitive members of DropAircraft,
// as opposed to DropAircraft / ACMass, which is what you would get by calling loadObject
// instead of loadRoot
QJsonObject ModuleLoader::loadRoot(QList<QString> keys) {
	QJsonObject rootObj = QJsonObject();
	for (QString key : keys) {
		QJsonObject resultObj = QJsonObject();
		QJsonObject defaultObj = QJsonObject();
		QJsonObject lookupObj = modulesObj[key].toObject();
		for (QString subKey : lookupObj.keys()) {
			bool isObject = lookupObj[subKey].type() == QJsonValue::Type::Object;
			bool isArray = lookupObj[subKey].type() == QJsonValue::Type::Array;
			bool isPrimitive = !isObject && !isArray;
			if (isPrimitive) {
				defaultObj.insert(subKey, lookupObj[subKey]);
			}
			else if (isObject) {
				resultObj.insert(subKey, loadObject(subKey, modulesObj));
			}
			else if (isArray) {
				resultObj.insert(subKey, loadArray(subKey, lookupObj[subKey]));
			}
		}
		resultObj.insert(key, defaultObj);
		rootObj.insert(key, resultObj);
	}
	return rootObj;
}

// note the calling interface is a bit weird between loadObject and loadArray
// for loadObject, you give the key to the object you want to load, and its parent object
// strange things happen that i don't totally understand when i try to change loadObject
// to receive a QJsonValue of the object being loaded, as opposed to a QJsonObject of the
// parent object and a key to the child to load
// this is an internal function anyways so it's not a big deal, but
// it's something to return to and figure out later if there's time
QJsonObject ModuleLoader::loadObject(QString key, QJsonObject parentObj) {
	qDebug() << "loadObject key = " << key;
	// resultObj is the recursive return value
	QJsonObject resultObj = QJsonObject();
	QJsonObject lookupObj = parentObj[key].toObject();
	// loop over the subcomponents
	for (QString key : lookupObj.keys()) {
		// if the subcomponent is a blank object, fill it out recursively
		if (lookupObj[key].type() == QJsonValue::Type::Object) {
			qDebug() << "loadObject object insert key = " << key;
			resultObj.insert(key, loadObject(key, modulesObj));
		}
		// if the subcomponent is an array, fill it out recursively
		else if (lookupObj[key].type() == QJsonValue::Type::Array) {
			qDebug() << "loadObject array insert key = " << key;
			resultObj.insert(key, loadArray(key, lookupObj[key]));
		}
		// otherwise, insert the primitive element
		else {
			qDebug() << "loadObject primitive insert key = " << key;
			resultObj.insert(key, lookupObj[key]);
		}
	}
	// for this to work, each leaf object has to be inserted to its parent object
	return resultObj;
}

// for loadArray, you pass a value to the actual array you are loading,
// and the moduleKey is for expanding object members in arrays
QJsonValue ModuleLoader::loadArray(QString moduleKey, QJsonValue value) {
	// resultArr is the recursive return value
	QJsonArray resultArr = QJsonArray();
	// lookupArr is the array to load from
	QJsonArray lookupArr = value.toArray();
	// iterate the array elements, and load blank objects based on the array key
	for (int i = 0, max = lookupArr.count(); i < max; i++) {
		// fill out any array element that is a blank object
		// as an object, recursively based on the top level array's key
		if (lookupArr[i].type() == QJsonValue::Type::Object) {
			resultArr.insert(i, loadObject(moduleKey, modulesObj));
		}
		// fill out nested arrays recursively
		else if (lookupArr[i].type() == QJsonValue::Type::Array) {
			resultArr.insert(i, loadArray(moduleKey, lookupArr[i].toArray()));
		}
		// otherwise, insert the primitive element
		else {
			resultArr.insert(i, lookupArr[i]);
		}
	}
	qDebug() << "loadArray array size = " << resultArr.count();
	// return the result as an object
	return resultArr;
}