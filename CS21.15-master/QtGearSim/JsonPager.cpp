#include <QWidget>
#include <QStackedWidget>
#include <QPushButton>
#include <QTreeView>
#include <QTableView>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QByteArray>
#include <QModelIndex>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonDocument>
#include <QString>
#include <QJsonArray>
#include <QLabel>
#include <tuple>
#include <QtGlobal>
#include "QJsonModel.h"
#include "JsonPager.h"
#include "..\dataReadingExample\GearSimDataTable.h"
#include "..\DataReadingExample\tableModel.h"

JsonPager::JsonPager(QWidget* parent = nullptr) : QWidget(parent) {
	//models = QList<QJsonModel*>();
	models = QList<QAbstractItemModel*>();
	stack = new QStackedWidget();

	back = new QPushButton("Back");
	next = new QPushButton("Next");

	hLayout = new QHBoxLayout();
	vLayout = new QVBoxLayout();
	vLayout->addWidget(stack, 1);
	vLayout->addLayout(hLayout);
	hLayout->addWidget(back, 1, Qt::AlignLeft);
	hLayout->addWidget(next, 1, Qt::AlignRight);
	setLayout(vLayout);

	connect(back, &QPushButton::clicked, [=]() {
		int idx = this->stack->currentIndex();
		int min = 0;
		if (idx > min) {
			this->stack->setCurrentIndex(idx - 1);
		}
	});
	connect(next, &QPushButton::clicked, [=]() {
		int idx = this->stack->currentIndex();
		int max = this->stack->count();
		if (idx < max) {
			this->stack->setCurrentIndex(idx + 1);
		}
	});
}

JsonPager::JsonPager(QJsonObject jsonObject, QWidget* parent) : JsonPager(parent) {
	//Remove arrays from json.
	QList<std::tuple<QString, QJsonValue>> arrays;
	jsonObject = getAndRemoveArrays(jsonObject, arrays);
	
	//nonarray iteration
	QStringList keys = jsonObject.keys();
	QStringList::const_iterator keyIt = keys.constBegin();
	QStringList::const_iterator keyItEnd = keys.constEnd();
	QJsonObject::const_iterator valueIt = jsonObject.constBegin();
	while (keyIt != keyItEnd) {
		QString key = *keyIt++;
		QJsonValue value = *valueIt++;
		QJsonDocument jsonDocument = QJsonDocument(value.toObject());

		QJsonModel* model = new QJsonModel(this);
		model->loadJson(jsonDocument);
		models.append(model);

		QLabel* label = new QLabel(key);
		QTreeView* view = new QTreeView();
		view->setModel(model);
		view->expandAll();
		view->setStyleSheet("QTreeView::branch {  border-image: url(none.png); }");
		view->setRootIsDecorated(false);
		view->setItemsExpandable(false);
		view->setHeaderHidden(true);

		DisplayWidget* w = new DisplayWidget(label, view);
		stack->addWidget(w);
	}
	
	//Array iteration
	QList<std::tuple<QString, QJsonValue>>::const_iterator arrIt = arrays.constBegin();
	QList<std::tuple<QString, QJsonValue>>::const_iterator arrItEnd = arrays.constEnd();
	while (arrIt != arrItEnd) {
		std::tuple<QString, QJsonValue> tup = *arrIt++;
		QString key = std::get<QString>(tup);
		QJsonValue value = std::get<QJsonValue>(tup);
		QJsonObject obj = value.toObject();

		if (obj.contains("data")) {
			QLabel* propertiesLabel = nullptr;
			QTreeView* propertiesView = nullptr;
			if (obj.contains("properties")) {
				QJsonValue propertiesValue = obj["properties"];
				QVariant propertiesVariant = propertiesValue.toVariant();
				QJsonDocument propertiesJsonDocument = QJsonDocument::fromVariant(propertiesVariant);
				qDebug() << propertiesJsonDocument.toJson();
				QJsonModel* propertiesModel = new QJsonModel(this);
				propertiesModel->loadJson(propertiesJsonDocument);
				models.append(propertiesModel);

				propertiesLabel = new QLabel("properties");
				propertiesView = new QTreeView();
				propertiesView->setModel(propertiesModel);
				propertiesView->expandAll();
				propertiesView->setStyleSheet("QTreeView::branch {  border-image: url(none.png); }");
				propertiesView->setRootIsDecorated(false);
				propertiesView->setItemsExpandable(false);
				propertiesView->setHeaderHidden(true);
				propertiesView->setColumnHidden(0, true);
			}

			//Extract array from the value at the current key.
			QJsonValue dataValue = obj["data"];
			QJsonArray dataArray = dataValue.toArray();
			//QJsonObject dataObject = QJsonObject(); //GearsimDataTable
			QList<QList<float>> data;

			for (int i = 0, max = dataArray.size(); i < max; i++) {
				//dataObject.insert(QString::number(i), dataArray.at(i)); //Old code for Oleg's example json array. //TODO: Remove this
				/* dataArray : [
					[A0, A1, A2],
					[B0, B1 B2,] 
				]*/
				QJsonArray subArray = dataArray.at(i).toArray();
				/*for (int j = 0; j < subArray.size(); j++) {
					//Build the new key
					key = QString::number(j);

					//Associate the key with the value
					dataObject.insert(key, subArray.at(j));
				}*/

				//Fill the array of arrays.
				QList<float> subList;
				for (QJsonValue j : subArray) {
					subList.append(j.toDouble());
				}
				data.append(subList);
			}
			QString tableName = "Name";
			QJsonValue colDataValue = obj["Column Names"];
			QJsonArray colDataArray = dataValue.toArray();
			QStringList colNames;
			for (QJsonValue k : colDataArray) {
				colNames.append(k.toString());
			}

			GearSimDataTable* table = new GearSimDataTable(tableName, data[0].size(), data, colNames);
			tableModel* tableModelTest = new tableModel(nullptr, table);
			/*QJsonDocument dataJsonDocument = QJsonDocument(dataObject);
			qDebug() << dataJsonDocument.toJson();
			QJsonModel* dataModel = new QJsonModel(this);
			dataModel->loadJson(dataJsonDocume
			nt);
			*/
			//models.append(dataModel);
			models.append(tableModelTest);

			QLabel* dataLabel = new QLabel(key);
			QTableView* dataView = new QTableView();
			//dataView->setModel(dataModel);
			dataView->setModel(tableModelTest);
			//dataView->horizontalHeader()->hide();
			//dataView->setColumnHidden(0, false);
			//dataView->setColumnWidth(0, 0);

			
			DisplayTableWidget* w = new DisplayTableWidget(dataLabel, propertiesLabel, dataView, propertiesView);
			stack->addWidget(w);
		}
		else {
			QJsonArray array = value.toArray();
			for (int i = 0, max = array.size(); i < max; i++) {
				obj.insert(QString::number(i), array.at(i));
			}
			QJsonDocument jsonDocument = QJsonDocument(obj);

			QJsonModel* model = new QJsonModel(this);
			model->loadJson(jsonDocument);
			models.append(model);

			QLabel* label = new QLabel(key);
			QTreeView* view = new QTreeView();
			view->setModel(model);
			view->expandAll();
			view->setStyleSheet("QTreeView::branch {  border-image: url(none.png); }");
			view->setRootIsDecorated(false);
			view->setItemsExpandable(false);
			view->setHeaderHidden(true);
			view->setColumnHidden(0, true);

			DisplayWidget* w = new DisplayWidget(label, view);
			stack->addWidget(w);
		}
	}
}

QJsonObject JsonPager::getAndRemoveArrays(QJsonObject jsonObject, QList<std::tuple<QString, QJsonValue>>& arrays) {
	QStringList keys = jsonObject.keys();
	QStringList::const_iterator keyIt = keys.constBegin();
	QStringList::const_iterator keyItEnd = keys.constEnd();
	QJsonObject::const_iterator valueIt = jsonObject.constBegin();

	QList<std::tuple<QString, QJsonValue>> arrs = QList<std::tuple<QString, QJsonValue>>();
	QList<std::tuple<QString, QJsonValue>> objs = QList<std::tuple<QString, QJsonValue>>();
	while (keyIt != keyItEnd) {
		QString key = *keyIt++;
		QJsonValue value = *valueIt++;
		QJsonObject obj = value.toObject();
		if (value.isArray() || obj.contains("data")) {
			std::tuple<QString, QJsonValue> tup = std::tuple<QString, QJsonValue>(key, value);
			arrs.append(tup);
		}
		else if (value.isObject()) {
			std::tuple<QString, QJsonValue> tup = std::tuple<QString, QJsonValue>(key, value);
			objs.append(tup);
		}
	}
	for (int i = 0, max = arrs.length(); i < max; i++) {
		std::tuple<QString, QJsonValue> tup = arrs[i];
		QString key = std::get<QString>(tup);
		QJsonValue value = std::get<QJsonValue>(tup);
		std::tuple<QString, QJsonValue> kv = std::tuple<QString, QJsonValue>(key, value);
		arrays.append(kv);
		jsonObject.remove(key);
	}
	for (int i = 0, max = objs.length(); i < max; i++) {
		std::tuple<QString, QJsonValue> tup = objs[i];
		QString key = std::get<QString>(tup);
		QJsonValue value = std::get<QJsonValue>(tup);
		QJsonObject obj = value.toObject();
		QJsonObject tmp = getAndRemoveArrays(obj, arrays);
		jsonObject.remove(key);
		jsonObject.insert(key, QJsonValue(tmp));
	}
	
	return jsonObject;
}