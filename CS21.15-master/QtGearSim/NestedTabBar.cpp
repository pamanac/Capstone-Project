#include "NestedTabBar.h"
#include "PrimaryWindow.h"
#include <QDebug>
#include <QLayout>
#include <QPushButton>
#include <QStackedWidget>
#include <qtabbar.h>
#include <tuple>

void NestedTabBar::setConnections(
	std::function<void(QString, QString)> currentChangedSlot,
	std::function<void(QString, QString)> tabCloseRequestedSlot)
{

	connect(primaryTabBar, &QTabBar::currentChanged, secondaryTabBarStack, &QStackedWidget::setCurrentIndex);
	connect(primaryTabBar, &QTabBar::currentChanged, this, &NestedTabBar::emitCurrentChanged);
	connect(this, &NestedTabBar::currentChanged, currentChangedSlot);

	for (int i = 0; i < primaryTabCount; i++)
	{
		QTabBar* secondaryTabBar = static_cast<QTabBar*>(secondaryTabBarStack->widget(i));
		connect(secondaryTabBar, &QTabBar::currentChanged, this, &NestedTabBar::emitCurrentChanged);
		connect(secondaryTabBar, &QTabBar::tabCloseRequested, this, &NestedTabBar::emitTabCloseRequested);
	}


	connect(this, &NestedTabBar::tabCloseRequested, tabCloseRequestedSlot);
	connect(this, &NestedTabBar::tabCloseRequested, this, &NestedTabBar::removeTab);

}

NestedTabBar::NestedTabBar(QList<QString>* primaryTabStrings, QWidget* parent)
	: QWidget(parent)
{
	// we use layout for ownership, so no parent pointers for constructors
	primaryTabBar = new QTabBar();
	primaryTabCount = primaryTabStrings->size();
	for (int i = 0; i < primaryTabCount; i++) {
		primaryTabBar->addTab(primaryTabStrings->at(i));
	}
	secondaryTabBarStack = new QStackedWidget();

	// initialize the secondary tab widgets, and add them to the stack
	for (int i = 0; i < primaryTabCount; i++)
	{
		auto secondaryTabBar = new QTabBar();
		secondaryTabBar->setTabsClosable(false);
		secondaryTabBar->setMovable(false);
		secondaryTabBarStack->addWidget(secondaryTabBar);
	}

	// primaryTabs is above subTabs
	verticalLayout = new QVBoxLayout();
	verticalLayout->addWidget(primaryTabBar);
	verticalLayout->addWidget(secondaryTabBarStack);
	
	// all widgets added to layout are owned by this
	setLayout(verticalLayout);
	primaryTabBar->setCurrentIndex(0);
}


QTabBar* NestedTabBar::currentSecondaryTabBar()
{
	return static_cast<QTabBar*>(secondaryTabBarStack->currentWidget());
}

QTabBar* NestedTabBar::getSecondaryTabBar(int index)
{
	return static_cast<QTabBar*>(secondaryTabBarStack->widget(index));
}

QString NestedTabBar::getSecondaryTabBarText(int index) {
	auto bar = currentSecondaryTabBar();
	return bar->tabText(index);
}

int NestedTabBar::currentSecondaryTabBarIndex() {
	return currentSecondaryTabBar()->currentIndex();
}

QString NestedTabBar::currentSecondaryTabBarText() {
	return currentSecondaryTabBar()->tabText(currentSecondaryTabBarIndex());
}

std::tuple<int, int> NestedTabBar::findTab(QString primaryText, QString secondaryText = "") {
	int i = 0;
	int max = primaryTabBar->count();
	while (i < max) {
		if (primaryText == primaryTabBar->tabText(i)) {
			break;
		}
		i++;
	}
	if (secondaryText == "") {
		return std::tuple<int, int>(i, 0);
	}
	auto secondaryTabBar = getSecondaryTabBar(i);
	int j = 0;
	max = secondaryTabBar->count();
	while (j < max) {
		if (secondaryText == secondaryTabBar->tabText(j)) {
			break;
		}
		j++;
	}
	return std::tuple<int, int>(i, j);
}

bool NestedTabBar::addTab(QString primaryText, QString secondaryText = "") {
	const bool addPrimaryTabEnabled = false;
	int primaryIndex;
	int secondaryIndex;
	auto tabIndices = findTab(primaryText, secondaryText);
	std::tie(primaryIndex, secondaryIndex) = tabIndices;
	auto secondaryTabBar = getSecondaryTabBar(primaryIndex);
	bool wasTabAdded = false;

	if (primaryTabFound(tabIndices) && secondaryText != "") {
		primaryTabBar->setCurrentIndex(primaryIndex);
		secondaryTabBar->addTab(secondaryText);
		wasTabAdded = true;
	}
	else if (secondaryText == "" && addPrimaryTabEnabled) {
		primaryTabBar->addTab(primaryText);
		wasTabAdded = true;
	}
	return wasTabAdded;
}

// TODO: make this function make more sense, don't copy paste, think through all the cases
bool NestedTabBar::addAndSelectTab(QString primaryText, QString secondaryText = "") {
	const bool addPrimaryTabEnabled = false;
	int primaryIndex;
	int secondaryIndex;
	auto tabIndices = findTab(primaryText, secondaryText);
	std::tie(primaryIndex, secondaryIndex) = tabIndices;
	auto secondaryTabBar = getSecondaryTabBar(primaryIndex);
	bool wasTabAdded = false;
	
	//Switch to tab if it already exists
	if (tabFound(tabIndices)) {
		primaryTabBar->setCurrentIndex(primaryIndex);
		secondaryTabBar->setCurrentIndex(secondaryIndex);
	}
	else if (primaryTabFound(tabIndices) && secondaryText != "") {
		primaryTabBar->setCurrentIndex(primaryIndex);
		secondaryTabBar->addTab(secondaryText);
		secondaryTabBar->setCurrentIndex(secondaryTabBar->count() - 1);
		wasTabAdded = true;
	}
	else if (secondaryText == "" && addPrimaryTabEnabled) {
		primaryTabBar->addTab(primaryText);
		primaryTabBar->setCurrentIndex(primaryTabBar->count() - 1);
		wasTabAdded = true;
	}
	return wasTabAdded;
}

bool NestedTabBar::removeTab(QString primaryText, QString secondaryText = "") {
	const bool removePrimaryTabEnabled = false;
	int primaryIndex;
	int secondaryIndex;
	auto tabIndices = findTab(primaryText, secondaryText);
	std::tie(primaryIndex, secondaryIndex) = tabIndices;
	auto secondaryTabBar = getSecondaryTabBar(primaryIndex);
	bool wasTabRemoved = false;

	if (tabFound(tabIndices)) {
		secondaryTabBar->removeTab(secondaryIndex);
		wasTabRemoved = true;
	}
	else if (primaryTabFound(tabIndices) && secondaryText == "" && removePrimaryTabEnabled) {
		primaryTabBar->removeTab(primaryIndex);
		wasTabRemoved = true;
	}
	return wasTabRemoved;
}

bool NestedTabBar::tabFound(std::tuple<int, int> tabIndices) {
	int primaryIndex;
	int secondaryIndex;
	std::tie(primaryIndex, secondaryIndex) = tabIndices;
	auto secondaryTabBar = getSecondaryTabBar(primaryIndex);
	return primaryIndex < primaryTabBar->count() && secondaryIndex < secondaryTabBar->count();
}

bool NestedTabBar::primaryTabFound(std::tuple<int, int> tabIndices) {
	int primaryIndex;
	int secondaryIndex;
	std::tie(primaryIndex, secondaryIndex) = tabIndices;
	auto secondaryTabBar = getSecondaryTabBar(primaryIndex);
	return primaryIndex < primaryTabBar->count();
}

QString NestedTabBar::currentPrimaryTabBarText() {
	return primaryTabBar->tabText(primaryTabBar->currentIndex());
}

void NestedTabBar::setCurrentWidgetOf(QStackedWidget* stack) {
	QString primaryText = currentPrimaryTabBarText();
	QString secondaryText = currentSecondaryTabBarText();
	auto widget = findWidget(stack, primaryText, secondaryText);
	qDebug() << "setCurrentWidgetOf" << stack->objectName() << primaryText << secondaryText << widget;
	stack->setCurrentWidget(widget);
}

QWidget* NestedTabBar::findWidget(QWidget* parent, QString primaryText, QString secondaryText) {
	QWidget* widget = parent->findChild<QWidget*>(primaryText + secondaryText);
	if (widget == nullptr) {
		widget = parent->findChild<QWidget*>(secondaryText);
	}
	qDebug() << "findWidget" << parent << primaryText << secondaryText << widget;
	return widget;
}

void NestedTabBar::emitTabCloseRequested(int index) {
	QString primaryText = currentPrimaryTabBarText();
	auto bar = currentSecondaryTabBar();
	QString secondaryText = getSecondaryTabBarText(index);
	emit tabCloseRequested(primaryText, secondaryText);
}

void NestedTabBar::emitCurrentChanged() {
	QString primaryText = currentPrimaryTabBarText();
	QString secondaryText = currentSecondaryTabBarText();
	emit currentChanged(primaryText, secondaryText);
}