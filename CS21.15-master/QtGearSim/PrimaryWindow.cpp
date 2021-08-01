#include "NestedTabBar.h"
#include "../custom_layout_and_qss/borderlayout.h"
#include "PrimaryWindow.h"
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QToolBar>
#include <QMenuBar>
#include <QTextEdit>
#include <QPixMap>
#include <QDebug>
#include <QSortFilterProxyModel>
#include <QList>


PrimaryWindow::PrimaryWindow(QWidget* parent) : QWidget(parent)
{
	primaryTabStrings = new QList<QString>;
	QList<QString> tabNames = { "Models", "Simulations", "Renders" };
	for (QString name : tabNames)
		primaryTabStrings->append(name);

	frameStack = new QStackedWidget();
	frameStack->setObjectName("frameStack");

	setWindowIcon(QIcon(":/data/Resources/GearSim.ico"));
	setWindowTitle("GearSim 2.8");

	logoLabel = insertLogo();
	topOptions = generateMenuToolbar();

	messages = new QTextEdit();
	messages->setPlainText("------------------------------------ \n");
	messages->append("-------- Welcome to GearSim 2.8 ------------ \n");
	messages->append("------------------------------ \n");

	tabBar = generateTabs();

	explorerWidget = generateExplorer();
	
	borderLayoutWidget = generateDisplayWidget();

	manageLayouts();	
}

PrimaryWindow::~PrimaryWindow() {
	delete primaryTabStrings;
}

Explorer* PrimaryWindow::generateExplorer() {
	auto selectionSlot = [=](QItemSelection const& selected) {
		for (auto const& index : selected.indexes()) {
			int primaryIndex = tabBar->primaryTabBar->currentIndex();
			QString text = explorerWidget->data(index);
			QString primaryText = tabBar->currentPrimaryTabBarText();
			QString secondaryText = text;
			secondaryText.replace(primaryText, "");
			if (tabBar->addTab(primaryText, secondaryText)) {
				QWidget* widget = new QPushButton(text);
				widget->setObjectName(text);
				frameStack->addWidget(widget);
				frameStack->setCurrentWidget(widget);
			}
			else {
				tabBar->setCurrentWidgetOf(frameStack);
			}
		}
	};

	QList<QStandardItem*> model_items;
	for (int row = 0, untilRow = primaryTabStrings->size(); row < untilRow; row++) {
		for (int col = 0; col < 3; col++) {
			auto ordinal = row * 3 + col + 1;
			auto primaryText = primaryTabStrings->at(row);
			auto secondaryText = QString("%0").arg(ordinal);
			qDebug() << primaryText << secondaryText;
			auto item = new QStandardItem(primaryText + secondaryText);
			model_items.append(item);
		}
	}

	QString filter = primaryTabStrings->at(0) + ".*";


	Explorer* myExplorer = new Explorer(this);
	myExplorer->fillModel(model_items);
	myExplorer->setFilter(filter);
	myExplorer->setConnections(selectionSlot);

	return myExplorer;

}

NestedTabBar* PrimaryWindow::generateTabs() {
	//
	// this avoids having to stuff references in all our classes
	//
	// an alternative is to just put connect calls after the constructor,
	// but that doesn't enforce using the class properly
	// (let the compiler check if we forgot to connect any slots)
	//
	// the main difference from the logic of the previous code is that
	// the work for the slot is split between PrimaryWindow and NestedTabBar
	// for example, secondaryTabBar->removeTab() is called in NestedTabBar
	// but the logic for frameStack and explorerStack is in PrimaryWindow
	// as local variable lambdas (wouldn't make sense as member functions)
	//
	auto currentChangedSlot = [=](QString primaryText, QString secondaryText) {
		tabBar->setCurrentWidgetOf(frameStack);
		// explorerStack is not currently used in any meaningful way!
		// tabBar->setCurrentWidgetOf(explorerStack);
		explorerWidget->setFilter(primaryText + ".*");
	};
	auto tabCloseRequestedSlot = [=](QString primaryText, QString secondaryText) {
		auto widget = tabBar->findWidget(frameStack, primaryText, secondaryText);
		frameStack->removeWidget(widget);
		delete widget;
	};
	NestedTabBar* myBar = new NestedTabBar(primaryTabStrings);
	myBar->setConnections(currentChangedSlot, tabCloseRequestedSlot);

	return myBar;
}


void PrimaryWindow::manageLayouts() {
	globalVertical = new QVBoxLayout();
	verticalLayout = new QVBoxLayout();
	horizontalLayout = new QHBoxLayout();
	globalHorizontal = new QHBoxLayout();

	globalVertical->addWidget(logoLabel, 0);
	globalVertical->addWidget(topOptions, 0);
	verticalLayout->addWidget(tabBar, 0);
	horizontalLayout->addWidget(explorerWidget, 1);
	horizontalLayout->addWidget(frameStack, 1);
	verticalLayout->addLayout(horizontalLayout, 1);
	globalHorizontal->addLayout(verticalLayout, 1);
	globalVertical->addLayout(globalHorizontal, 0);

	verticalLayout->addWidget(messages, 0);
	globalHorizontal->addWidget(borderLayoutWidget, 0);
	setLayout(globalVertical);
}

QLabel* PrimaryWindow::insertLogo() {
	QLabel* logoLabel = new QLabel();
	QPixmap logo;
	if (!logo.load(":/data/Resources/Logo.PNG")) {
		qWarning("Failed to load logo");
	}
	logoLabel->setPixmap(logo);
	return logoLabel;
}

QMenuBar* PrimaryWindow::generateMenuToolbar() {
	QMenuBar* menu = new QMenuBar();
	QMenu* fileMenu = new QMenu(tr("&File"), menu);
	QAction* newFile = new QAction(tr("&New..."), fileMenu);
	newFile->setShortcuts(QKeySequence::New);
	QAction* openFile = new QAction(tr("&Open..."), fileMenu);
	openFile->setShortcuts(QKeySequence::Open);
	QAction* saveFile = new QAction(tr("&Save..."), fileMenu);
	saveFile->setShortcuts(QKeySequence::Save);
	QAction* prefFile = new QAction(tr("&GearSim Preferences..."), fileMenu);
	prefFile->setShortcuts(QKeySequence::Preferences);
	QAction* closeFile = new QAction(tr("&Close..."), fileMenu);
	closeFile->setShortcuts(QKeySequence::Close);
	fileMenu->addAction(newFile);
	fileMenu->addAction(openFile);
	fileMenu->addAction(saveFile);
	fileMenu->addAction(prefFile);
	fileMenu->addAction(closeFile);
	menu->addMenu(fileMenu);
	QMenu* dataMenu = new QMenu(tr("&Data"), menu);
	QAction* compareWSaveAct = new QAction(tr("&Compare current inputs with save file..."), dataMenu);
	dataMenu->addAction(compareWSaveAct);
	QAction* compareWFileAct = new QAction(tr("&Compare inputs of two saved files..."), dataMenu);
	dataMenu->addAction(compareWFileAct);
	QAction* geneticAct = new QAction(tr("&Perform genetic algorithm..."), dataMenu);
	dataMenu->addAction(geneticAct);
	QAction* defineAct = new QAction(tr("&Define parameter..."), dataMenu);
	dataMenu->addAction(defineAct);
	menu->addMenu(dataMenu);
	QMenu* simulationMenu = new QMenu(tr("&Simulation"), menu);
	QAction* simDetailsAct = new QAction(tr("&Solver and simulation details"), simulationMenu);
	simulationMenu->addAction(simDetailsAct);
	QAction* simWorkplaceAct = new QAction(tr("&Manage 'To Workspace' Blocks..."), simulationMenu);
	simulationMenu->addAction(simWorkplaceAct);
	QAction* simBatchAct = new QAction(tr("&Perform batch analysis..."), simulationMenu);
	simulationMenu->addAction(simBatchAct);
	menu->addMenu(simulationMenu);
	QMenu* helpMenu = new QMenu(tr("&Help"), menu);
	QAction* helpUserAct = new QAction(tr("User Manual"), helpMenu);
	helpMenu->addAction(helpUserAct);
	QAction* helpTheoryAct = new QAction(tr("Theory Manual"), helpMenu);
	helpMenu->addAction(helpTheoryAct);
	menu->addMenu(helpMenu);

	return menu;
}

QWidget* PrimaryWindow::generateDisplayWidget() {
	// dummy border layout
	borderLayoutWidget = new QWidget();
	BorderLayout* borderLayout = new BorderLayout();
	//QPushButton* center = new QPushButton("Center");
	QLabel* east = new QLabel("East");
	QLabel* west = new QLabel("West");
	QPushButton* simulate_button = new QPushButton("Simulate");
	QComboBox* dropDown = new QComboBox();
	dropDown->insertItem(0, "Whole Aircraft");
	dropDown->insertItem(1, "Wheel View");

	QLabel* modelOne = new QLabel();
	QPixmap M1(":/data/Resources/Model1.PNG");
	modelOne->setPixmap(M1);
	QLabel* modelTwo = new QLabel();
	QPixmap M2(":/data/Resources/Model2.PNG");
	modelTwo->setPixmap(M2);
	imageStack = new QStackedWidget(this);
	imageStack->addWidget(modelOne);
	imageStack->addWidget(modelTwo);

	connect(dropDown, QOverload<int>::of(&QComboBox::currentIndexChanged), imageStack, &QStackedWidget::setCurrentIndex);

	borderLayout->addWidget(imageStack, BorderLayout::Center);
	borderLayout->addWidget(dropDown, BorderLayout::North);
	borderLayout->addWidget(west, BorderLayout::West);
	borderLayout->addWidget(east, BorderLayout::East);
	borderLayout->addWidget(simulate_button, BorderLayout::South);
	borderLayoutWidget->setLayout(borderLayout);

	// name it
	borderLayoutWidget->setObjectName("Display");

	return borderLayoutWidget;
}
