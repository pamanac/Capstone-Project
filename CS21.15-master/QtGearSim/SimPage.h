#ifndef SIM_PAGE_H
#define SIM_PAGE

#include <QtWidgets>
#include <QSortFilterProxyModel>
#include "GenericPage.h"
#include "MainWindow.h"

class SimPage : public GenericPage {
	Q_OBJECT
public:
	SimPage(QString aircraftSelectedName, QString directorySelectedName, MainWindow* parent = 0);
	~SimPage();

	void nextPage() override;
private:
	//Window group
	QStackedWidget* group;

	//Layouts
	QGridLayout* gridLayout;

	//Labels
	QLabel* labelDirectory;
	QLabel* labelAircraftSelected;
	QLabel* labelTestDescription;
	
	//Models and related views
	QListView* simulationsList;
	QListView* simulationsOptionsList;
	QStandardItemModel* simulationsModel;
	QStandardItemModel* simulationsOptionsModel;
	QSortFilterProxyModel* optionProxy;
};

#endif