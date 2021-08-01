#ifndef NON_TABLE_PROXY_MODEL_H
#define NON_TABLE_PROXY_MODEL_H

#include <QSortFilterProxyModel>
#include <QWidget>

#include "QJsonModel.h"
/*
 * NonTableProxyModel filters out all arrays from the source json model
 */
class NonTableProxyModel : public QSortFilterProxyModel {
public:
	NonTableProxyModel(QWidget* parent = nullptr);

protected:
	bool filterAcceptsRow(int row, QModelIndex const& parent) const override;
};
#endif