#include "TabProxyModel.h"

TabProxyModel::TabProxyModel(QString primaryText, QString secondaryText, QWidget* parent) : QSortFilterProxyModel(parent), primaryText(primaryText), secondaryText(secondaryText) {}

bool TabProxyModel::filterAcceptsRow(int row, QModelIndex const& parent) const {
	QJsonModel* model = static_cast<QJsonModel*>(sourceModel());
	QModelIndex idx = sourceModel()->index(row, 0, parent);
	QJsonTreeItem* item = model->item(idx);
	QJsonTreeItem* parentItem = item->parent();

	// match root level primary keys
	if (item->key() == primaryText && parentItem != nullptr && parentItem->key() == "root") {
		return true;
	}
	// match secondary keys and anything contained within them
	while (parentItem != nullptr) {
		if (item->key() == secondaryText && parentItem != nullptr && parentItem->key() == primaryText) {
			return true;
		}
		item = parentItem;
		parentItem = parentItem->parent();
	}
	// anything else doesn't match
	return false;
}