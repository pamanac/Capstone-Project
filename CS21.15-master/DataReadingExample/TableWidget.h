#pragma once
#include<QWidget>
#include <QStandardItemModel>
#include <qtableview.h>
#include "GearSimDataTable.h"
#include "tableModel.h"

class tableWidget : public QWidget {
	Q_OBJECT
public:
	tableWidget(QWidget* parent, GearSimDataTable* data);

private:
	tableModel* model;
	QTableView* view;
};