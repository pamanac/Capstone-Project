#ifndef DATA_ENTRY_PAGE_H
#define DATA_ENTRY_PAGE_H

#include <qwidget.h>
#include "qjsonmodel.h"
#include <qlabel.h>
#include <qpushbutton.h>
#include <qlistview.h>
#include <qcombobox.h>
#include <qstackedwidget.h>
#include <qlayout.h>
#include "../DataReadingExample/multiPager.h"
#include <QStandardItemModel>
#include "GenericPage.h"
#include "MainWindow.h"
#include "NestedTabBar.h"
#include "ModuleLoader.h"
/*
	DataEntryPage is page for handling user interaction with component data.

	The most important object types in this object are the multipager, QListView, and QStandardItemModel.
	The multipager handles the display of the contents of GearSimDataFiles which are loaded from filemodel.
*/
class DataEntryPage : public GenericPage {
	Q_OBJECT
private:
	QStackedWidget* group;
	QHBoxLayout* horiz;
	QVBoxLayout* vert;
	
	QComboBox* testTypes;
	QLabel* aircraft;

	multiPager* pager;
	QListView* componentsView;
	QStandardItemModel* filemodel;
	NestedTabBar* tabs;

	QList<QAbstractProxyModel*> proxyList;
	QStackedWidget* viewStack;

public:
	DataEntryPage(MainWindow* parent = 0);
	~DataEntryPage();

	ModuleLoader moduleLoader;

	void nextPage() override;
};

#endif
