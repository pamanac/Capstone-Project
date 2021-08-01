#ifndef NESTED_TAB_BAR_H
#define NESTED_TAB_BAR_H

#include <QWidget>
#include <QTabBar>
#include <QTabWidget>
#include <QVector>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QSortFilterProxyModel>

#include <functional>

/*
This class is intended to implement a nested tab setup.

Requirements:
	One primary tab bar.
		Size must be statically sized.
		Each tab in primary bar has a sub-tab set.
			Sub-tabs must be dynamically sized.
			Nested tabs must be editable on the fly; state changes of subtabs are accepted.
		Each primary tab must remember which tab was last open.
*/
class NestedTabBar : public QWidget {
	Q_OBJECT
public:
	// public members for now
	size_t primaryTabCount;

	QVBoxLayout* verticalLayout;
	QTabBar* primaryTabBar;
	QStackedWidget* secondaryTabBarStack;

	NestedTabBar(QList<QString>* primaryTabStrings, QWidget* parent = nullptr);
	~NestedTabBar() = default;

	QTabBar* currentSecondaryTabBar();
	int currentSecondaryTabBarIndex();
	QString currentSecondaryTabBarText();

	QString currentPrimaryTabBarText();

	QTabBar* getSecondaryTabBar(int index);
	QString getSecondaryTabBarText(int index);

	QWidget* findWidget(QWidget* parent, QString primaryText, QString secondaryText);
	void setCurrentWidgetOf(QStackedWidget* stack);

	std::tuple<int, int> findTab(QString primaryText, QString secondaryText);

	void emitCurrentChanged();
	void emitTabCloseRequested(int index);

	bool addTab(QString primaryText, QString secondaryText);
	bool addAndSelectTab(QString primaryText, QString secondaryText);
	bool removeTab(QString primaryText, QString secondaryText);

	bool tabFound(std::tuple<int, int> indices);
	bool primaryTabFound(std::tuple<int, int> indices);

	void setConnections(
		std::function<void(QString, QString)> currentChangedSlot = [](QString, QString) {},
		std::function<void(QString, QString)> tabCloseRequestedSlot = [](QString, QString) {}
	);

signals:
	void currentChanged(QString primaryText, QString secondaryText);
signals:
	void tabCloseRequested(QString primaryText, QString secondaryText);
};

#endif