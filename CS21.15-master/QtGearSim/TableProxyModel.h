#ifndef TABLE_PROXY_MODEL_H
#define TABLE_PROXY_MODEL

#include <QSortFilterProxyModel>
#include <QWidget>
// QJsonModel dependency should be removed
#include "QJsonModel.h"
/*
 * TableProxyModel keeps all arrays and their parents (recursively up to the root) from the source json model
 *
 * keepItems has to be kept track of by connecting the source model's insertRows and dataChanged signals to
 * update keepItems
 */
class TableProxyModel : public QSortFilterProxyModel {
public:
	QSet<QJsonTreeItem*> keepItems;
	TableProxyModel(QWidget* parent = nullptr);

	// this is a helper function for debug prints
	// QString getPath(QModelIndex idx) const;

	void setSourceModel(QAbstractItemModel* model) override;

protected:
	bool filterAcceptsRow(int row, const QModelIndex& parent) const override;
};
#endif