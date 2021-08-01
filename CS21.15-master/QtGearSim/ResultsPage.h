#ifndef RESULTS_PAGE_PLACEHOLDER_H
#define RESULTS_PAGE_PLACEHOLDER_H

#include <QWidget>
#include <QStackedWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QJsonObject>
#include <QTreeView>
#include <QSortFilterProxyModel>
#include <QVariant>
#include <QHeaderView>
#include <QScrollBar>
#include <QLabel>
#include <QPixmap>
#include <QBitmap>

#include "QJsonModel.h"
#include "JsonPager.h"
#include "GenericPage.h"
#include "MainWindow.h"

#define PLACEHOLDER_SELECTIONS_TEXT "{ \
\"Generated Plots & Graphs\" : { \
	\"Oleo Behavior\" : [\"Closure / Time\", \"Velocity / Time\", \"Force / Time\"], \
	\"Tire Vertical Response\" : {}, \
	\"Breaking Response\" : {} \
} \
}"

class ResultsPage : public GenericPage {
	Q_OBJECT
private:
	//Create subclass for definind proxy model features.
	class FilterProxyModel : public QSortFilterProxyModel {
		bool filterAcceptsColumn(int column, QModelIndex const& idx) const override {
			QJsonTreeItem* item = static_cast<QJsonTreeItem*>(idx.internalPointer());
			return !(idx.isValid() && item->type() == QJsonValue::Array && column == 0);
		}
	};

	//Button members
	QPushButton* plotBack;
	QPushButton* plotNext;

	//Stacked widgets
	QStackedWidget* plotStack;
	QStackedWidget* group;
	
	//Layouts
	QVBoxLayout* vLayoutPlots;
	QHBoxLayout* hLayout;
	QVBoxLayout* vLayout;
	QHBoxLayout* hLayoutPlotButtons;
	
	//Models and views
	QTreeView* selectionsView;
	QJsonModel* model;
	FilterProxyModel* proxy;

public:
	ResultsPage(MainWindow* parent = nullptr);

	//Refer to GenericPage.h for more info on nextPage().
	void nextPage() override;

	~ResultsPage();
};

#endif