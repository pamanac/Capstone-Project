#pragma once
#include "qpoint.h"
#include "qstandarditemmodel.h"
#include "GearSimDataTable.h"

class tableModel : public QAbstractTableModel {
	GearSimDataTable* tableData;

public:
	tableModel(QObject* parent, GearSimDataTable* data);
	int rowCount(const QModelIndex&) const override;
	int columnCount(const QModelIndex&) const override;

	//Delegate functions
	QVariant data(const QModelIndex&, int role) const override;
	Qt::ItemFlags flags(const QModelIndex& index) const override;
	bool setData(const QModelIndex& index, const QVariant& value, int role) override;

	QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
};