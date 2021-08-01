#include "NonTableProxyModel.h"

NonTableProxyModel::NonTableProxyModel(QWidget* parent) : QSortFilterProxyModel(parent) {}

bool NonTableProxyModel::filterAcceptsRow(int row, const QModelIndex& parent) const {
	QJsonModel* model = static_cast<QJsonModel*>(sourceModel());
	QModelIndex idx = sourceModel()->index(row, 0, parent);
	QJsonTreeItem* item = model->item(idx);
	return item->type() != QJsonValue::Type::Array;
}