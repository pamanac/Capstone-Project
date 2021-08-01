#include "Explorer.h"
#include <qdebug.h>

Explorer::Explorer(QWidget* parent = nullptr) : QWidget(parent) {

	view = new QListView(this);
	model = new QStandardItemModel();
	proxyModel = new QSortFilterProxyModel();
	proxyModel->setSourceModel(model);
	view->setModel(proxyModel);
	layout = new QHBoxLayout();
	layout->addWidget(view);
	this->setLayout(layout);
}

void Explorer::fillModel(QList<QStandardItem*> const& items) {
	for (QStandardItem* item : items) {
		model->appendRow(item);
	}
}

void Explorer::setFilter(QString regExp) {
	proxyModel->setFilterRegularExpression(regExp);
}

QSortFilterProxyModel* Explorer::getProxyModel() {
	return proxyModel;
}

QString Explorer::data(QModelIndex index) {
	return proxyModel->data(index, Qt::DisplayRole).toString();
}

void Explorer::setConnections(
	std::function<void(QItemSelection const& selected)> onViewSelectionChange
) {
	connect(view->selectionModel(), &QItemSelectionModel::selectionChanged, onViewSelectionChange);
}
