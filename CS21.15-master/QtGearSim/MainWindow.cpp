#include "MainWindow.h"
#include <qdockwidget.h>
#include <qpushbutton.h>
#include <qlistview.h>
#include <qstandarditemmodel.h>
#include <qstackedwidget.h>
#include "SplashPage.h"
#include "ExampleBrowser.h"
#include "GenericPage.h"
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
	programState.userSelections.airplane = "";
	programState.userSelections.simulation = "";

	this->setWindowTitle("GearSim");
	dock = new QDockWidget("Dock Widget", this);
	dock->setObjectName("MainWDock");
	QListView* view = new QListView();
	view->setObjectName("DockList");
	qDebug() << "MainWindow address: " << this;
	QStackedWidget* navigatorStack = new QStackedWidget(nullptr);
	qDebug() << "Stack address: " << navigatorStack;
	this->stack = navigatorStack;
	SplashPage* splash = new SplashPage(this);
	
	QScreen* screen = QGuiApplication::primaryScreen();
	QRect  screenGeometry = screen->geometry();
	int height = screenGeometry.height();
	int width = screenGeometry.width();
	//this->resize(width, height - 60);
	this->showMaximized();
	navigatorStack->addWidget(splash);
	navigatorStack->setObjectName("CentralWidget");
	splash->setStyleSheet(ExampleBrowser::returnStyleSheet(":/data/Resources/splashScreen.qss"));

	//Fill the dock with pages.  Note that order matters.  These names must also match the page's object name.
	QStandardItemModel* model = new QStandardItemModel();
	model->appendRow(new QStandardItem("Splash page"));
	model->appendRow(new QStandardItem("Aircraft page"));
	model->appendRow(new QStandardItem("Sim page"));
	model->appendRow(new QStandardItem("Data entry page"));
	model->appendRow(new QStandardItem("Results page"));
	/* This old code fills the dock with "Page 1", "Page 2", etc.
	QStandardItemModel* model = new QStandardItemModel();
	for (int i = 0; i < 5; i++) {
		QString a = "Page " + QString::number(i);
		model->appendRow(new QStandardItem(a));
	}
	*/
	

	view->setModel(model);
	view->setEditTriggers(QAbstractItemView::NoEditTriggers);
	dock->setWidget(view);
	view->setCurrentIndex(view->model()->index(0, 0)); //Default selected dock index of page 0.
	dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	this->addDockWidget(Qt::LeftDockWidgetArea, dock);

	connect(view->selectionModel(), &QItemSelectionModel::selectionChanged, [=](const QItemSelection& selected) {
		for (auto& index : selected.indexes()) {
			QString name = index.data().toString();
			navigatorStack->setCurrentWidget(navigatorStack->findChild<QWidget*>(name));
		}
	});

	this->setMenuBar(generateMenu());
	this->setCentralWidget(navigatorStack);
	this->setWindowIcon(QIcon(":/data/Resources/GearSim.ico"));

	this->setStyleSheet(ExampleBrowser::returnStyleSheet(":/data/Resources/MainWindow.qss"));
}

QMenuBar* MainWindow::generateMenu() {
	QMenuBar* menu = new QMenuBar();
	QMenu* fileMenu = new QMenu(tr("&File"), menu);
	QAction* openFile = new QAction(tr("&Open..."), fileMenu);
	openFile->setShortcuts(QKeySequence::Open);
	QAction* saveFile = new QAction(tr("&Save..."), fileMenu);
	saveFile->setShortcuts(QKeySequence::Save);
	QAction* prefFile = new QAction(tr("&GearSim Preferences..."), fileMenu);
	prefFile->setShortcuts(QKeySequence::Preferences);
	QAction* closeFile = new QAction(tr("&Close..."), fileMenu);
	closeFile->setShortcuts(QKeySequence::Close);
	fileMenu->addAction(openFile);
	fileMenu->addAction(saveFile);
	fileMenu->addAction(prefFile);
	fileMenu->addAction(closeFile);
	//menu->addMenu(fileMenu);


	QMenu* viewMenu = new QMenu(tr("&View"), menu);
	QAction* closeDock = this->getDock()->toggleViewAction();
	closeDock->setShortcuts(QKeySequence::Close);
	viewMenu->addAction(closeDock);
	menu->addMenu(viewMenu);


	QMenu* dataMenu = new QMenu(tr("&Data"), menu);
	QAction* compareWSaveAct = new QAction(tr("&Compare current inputs with save file..."), dataMenu);
	dataMenu->addAction(compareWSaveAct);
	QAction* compareWFileAct = new QAction(tr("&Compare inputs of two saved files..."), dataMenu);
	dataMenu->addAction(compareWFileAct);
	QAction* geneticAct = new QAction(tr("&Perform genetic algorithm..."), dataMenu);
	dataMenu->addAction(geneticAct);
	QAction* defineAct = new QAction(tr("&Define parameter..."), dataMenu);
	dataMenu->addAction(defineAct);
	//menu->addMenu(dataMenu);

	QMenu* simulationMenu = new QMenu(tr("&Simulation"), menu);
	QAction* simDetailsAct = new QAction(tr("&Solver and simulation details"), simulationMenu);
	simulationMenu->addAction(simDetailsAct);
	QAction* simWorkplaceAct = new QAction(tr("&Manage 'To Workspace' Blocks..."), simulationMenu);
	simulationMenu->addAction(simWorkplaceAct);
	QAction* simBatchAct = new QAction(tr("&Perform batch analysis..."), simulationMenu);
	simulationMenu->addAction(simBatchAct);
	//menu->addMenu(simulationMenu);

	QMenu* helpMenu = new QMenu(tr("&Help"), menu);
	QAction* helpUserAct = new QAction(tr("User Manual"), helpMenu);
	helpMenu->addAction(helpUserAct);
	QAction* helpTheoryAct = new QAction(tr("Theory Manual"), helpMenu);
	helpMenu->addAction(helpTheoryAct);
	menu->addMenu(helpMenu);

	return menu;
}

QStackedWidget* MainWindow::getStack() {
	return stack;
}

QDockWidget* MainWindow::getDock() {
	return dock;
}

MainWindow::~MainWindow() {
	delete dock;
	delete stack; //We do take ownership of this.
};