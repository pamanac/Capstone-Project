#pragma once
#include <qwidget.h>
#include <QStandardItemModel>
#include "ExampleDataLineWidget.h"
#include "GearSimDataTable.h"
#include "GearSimDataFile.h"
#include "tableModel.h"
#include <qtableview.h>
#include "qtreeview.h"
#include "TreeModel.h"
#include "SDelegate.h"
#include <qlayout.h>
#include <QPushButton>
#include <qstackedwidget.h>

class multiPager : public QWidget {
	Q_OBJECT
public:
	multiPager(QWidget* parent = nullptr);
	multiPager(GearSimDataFile* file, QWidget* parent = nullptr);
	~multiPager();

	/*
	* Will change the workingFile variable if and only if the new value passes the isValidFile() check on the file.
	* Failure to change the variable results in a false.
	*/
	bool setFile(const QString &fileName);
	bool setFile(GearSimDataFile* file);
public slots:
	
private:
	QList<GearSimDataTable*> tables;
	QList<tableModel*> tmodels;
	QList<QTableView*> tviews;
	QList<GearSimData*> data;
	SDelegate* deleg;
	TreeModel* dmodel;
	QTreeView* dview;
	QStackedWidget* stack;
	QPushButton* next;
	QPushButton* save;

	GearSimDataFile* workingFile;

	QVBoxLayout* layout;

	void init(GearSimDataFile* file);
};