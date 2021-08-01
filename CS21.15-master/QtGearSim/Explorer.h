#ifndef EXPLORER_H
#define EXPLORER_H

#include <QWidget>
#include <QListView>
#include <QStackedWidget>
#include <QStandardItemModel>
#include <QLayout>
#include <QSortFilterProxyModel>

class Explorer : public QWidget {
	Q_OBJECT
private:
	QListView* view;
	QStandardItemModel* model;
	QSortFilterProxyModel* proxyModel;
	QHBoxLayout* layout;
public:
	Explorer(QWidget* parent);

	void setFilter(QString regExpr);
	QSortFilterProxyModel* getProxyModel();
	void fillModel(const QList<QStandardItem*> &items);
	QString data(QModelIndex index);

	void setConnections(
		std::function<void(QItemSelection const& selected)> onViewSelectionChange
	);
};

#endif