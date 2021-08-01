#include "TableProxyModel.h"

TableProxyModel::TableProxyModel(QWidget* parent) : QSortFilterProxyModel(parent) {}

// this is a helper function for debug prints
/*
QString TableProxyModel::getPath(QModelIndex idx) const {
	QJsonModel* model = static_cast<QJsonModel*>(sourceModel());
	QJsonTreeItem* item = model->item(idx);
	QList<QString> path = QList<QString>();
	QString strPath = "";
	while (item != nullptr) {
		path.append(item->key());
		item = item->parent();
	}
	for (int i = 0; i < path.count(); i++) {
		strPath += "/" + path[path.count() - 1 - i];
	}
	return strPath;
}
*/

void TableProxyModel::setSourceModel(QAbstractItemModel* model) {
	// keep track of keepItems as the source model is populated;
	connect(model, &QAbstractItemModel::rowsInserted, [=](QModelIndex const& parent, int first, int last) {
		for (int i = first; i <= last; i++) {
			// uncomment for debug prints
			/*
			QModelIndex key = model->index(i, 0, parent);
			QModelIndex value = model->index(i, 1, parent);
			qDebug() << "row added: " << model->data(key, Qt::DisplayRole) << model->data(value, Qt::DisplayRole);
			*/

			// this should be rewritten to use a public interface functions that have to be supplied by the model,
			// so this code is not specific just to QJsonModel
			QModelIndex idx = model->index(i, 0, parent);
			QJsonTreeItem* item = static_cast<QJsonModel*>(model)->item(idx);

			// bool isArray = item != nullptr && item->type() == QJsonValue::Array;
			bool isArrayElement = item != nullptr && item->parent() != nullptr && item->parent()->type() == QJsonValue::Array;

			if (isArrayElement) {
				while (item != nullptr) {
					if (keepItems.contains(item) == false) {
						keepItems.insert(item);
						// uncomment for debug prints
						/*
						QList<QString> path = QList<QString>();
						path.append(item->key());
						QJsonTreeItem* innerItem = item->parent();
						while (innerItem != nullptr) {
							path.append(innerItem->key());
							innerItem = innerItem->parent();
						}
						QString strPath = "";
						for (int i = 0; i < path.count(); i++) {
							strPath += "/" + path[path.count() - 1 - i];
						}
						*/
						// qDebug() << "tracking: " << strPath;
					}
					item = item->parent();
				}
			}
		}
	});
	// QJsonModel does not implement columnsInserted, which is not useful for this model
	/*
	connect(model, &QAbstractItemModel::columnsInserted, [=](QModelIndex const& parent, int first, int last) {
		for (int i = first; i <= last; i++) {
			QModelIndex idx = model->index(0, i, parent);
			qDebug() << "col added: " << model->data(idx, Qt::DisplayRole);
		}
	});
	*/
	// this is a stub, and it's necessary to update keepItems if data changes (if the model is altered after loadJson is called)
	connect(model, &QAbstractItemModel::dataChanged, [=](QModelIndex const& topLeft, QModelIndex const& botRight, QList<int> const& roles) {
		qDebug() << "data changed";
	});
	QSortFilterProxyModel::setSourceModel(model);
}

bool TableProxyModel::filterAcceptsRow(int row, const QModelIndex& parent) const {
	QJsonModel* model = static_cast<QJsonModel*>(sourceModel());
	QModelIndex idx = sourceModel()->index(row, 0, parent);
	QJsonTreeItem* item = model->item(idx);
	return keepItems.contains(item);
}