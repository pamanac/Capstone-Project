#ifndef PRIMARY_WINDOW_H
#define PRIMARY_WINDOW_H

#include "NestedTabBar.h"
#include <QWidget>
#include <QTabBar>
#include <QStackedWidget>
#include <QStandardItemModel>
#include <QListView>
#include <QMenuBar>
#include <QLabel>
#include <QTextEdit>
#include "Explorer.h"
#include <QSortFilterProxyModel>

class PrimaryWindow : public QWidget {
	Q_OBJECT

public:
	PrimaryWindow(QWidget* parent = 0);
	~PrimaryWindow();

private:
	QWidget* borderLayoutWidget;

	QList<QString>* primaryTabStrings;
	NestedTabBar* tabBar;

	QStackedWidget* frameStack;
	QStackedWidget* imageStack;
	QVBoxLayout* verticalLayout;
	QVBoxLayout* globalVertical;
	QHBoxLayout* horizontalLayout;
	QHBoxLayout* globalHorizontal;
	Explorer* explorerWidget;

	QMenuBar* topOptions;
	QLabel* logoLabel;

	QTextEdit* messages;

	QMenuBar* generateMenuToolbar();
	NestedTabBar* generateTabs();
	Explorer* generateExplorer();
	QLabel* insertLogo();
	QWidget* generateDisplayWidget();

	void manageLayouts();

};

#endif