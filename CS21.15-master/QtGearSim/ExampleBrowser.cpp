#include"ExampleBrowser.h"
#include "SplashPage.h"
#include "MainWindow.h"
#include "PrimaryWindow.h"
#include "JsonPager.h"
#include <QMenuBar>
#include <QByteArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QSet>
#include <QList>
#include <QTimer>

#include "TableProxyModel.h"
#include "NonTableProxyModel.h"
#include "ResultsPage.h"
#include "ProgressLabel.h"

/*
The following includes are not valid, since they lead to redefinitions.
However, they are helpful to acknowledge where examples are located.
#include"..\custom_layout_and_qss\ExampleCustomLayout.cpp"
#include"..\model_index_basic\ExampleModelViewIndex.cpp"
#include"..\model_view_basic\ExampleModelViewBasic.cpp"
*/

//The example browser constructor.  This widget is a simple window for running the various examples made by CS21.15
ExampleBrowser::ExampleBrowser() {
	//Create the buttons.
	buttonCustomLayout = new QPushButton("Example: Custom layout", this);
	buttonModelViewBasic = new QPushButton("Example: Basic model view", this);
	buttonModelViewIndex = new QPushButton("Test: Index-based model view", this);
	buttonExampleDataReading = new QPushButton("Example: GearSim data reading", this);
	buttonGearSim = new QPushButton("Prototype: Tabbed GearSim", this);
	buttonMatLab = new QPushButton("Example: MATLAB connections", this);
	buttonWireFrame = new QPushButton("Prototype: Linear GearSim", this);
	buttonJsonPager = new QPushButton("Example: Json Pager", this);

	buttonFilters2 = new QPushButton("Example: Filters2", this);
	buttonProgress = new QPushButton("Example: Progress", this);

	//Set tooltip descriptions
	this->setToolTip("This is the example viewer widget!");
	buttonCustomLayout->setToolTip("Example of how to implement custom layout");
	buttonModelViewBasic->setToolTip("Basic data model viewer example");
	buttonModelViewIndex->setToolTip("Data model index test");
	buttonExampleDataReading->setToolTip("GearSim Data reading");
	buttonGearSim->setToolTip("GearSim prototype");
	buttonWireFrame->setToolTip("GearSim prototype");
	buttonMatLab->setToolTip("MATLAB connection testing");
	buttonJsonPager->setToolTip("Json Pager Widget");
	buttonFilters2->setToolTip("Filters Widget");
	buttonProgress->setToolTip("Progress Widget");
	
	//Connect the buttons to their actual functions.
	connect(buttonCustomLayout, &QPushButton::clicked, [=]() {ExamplesNamespace::ExampleCustomLayout();});
	connect(buttonModelViewBasic, &QPushButton::clicked, [=]() {ExamplesNamespace::ExampleModelViewBasic();});
	connect(buttonModelViewIndex, &QPushButton::clicked, [=]() {ExamplesNamespace::ExampleModelViewIndex();});
	connect(buttonExampleDataReading, &QPushButton::clicked, [=]() {ExamplesNamespace::ExampleDataReading();});
	connect(buttonWireFrame, &QPushButton::clicked, [=]() {wireFrame();});
	connect(buttonGearSim, &QPushButton::clicked, [=]() {GearSim();});
	connect(buttonJsonPager, &QPushButton::clicked, [=]() {jsonPager();});
	connect(buttonProgress, &QPushButton::clicked, [=]() {progress(); });
	connect(buttonFilters2, &QPushButton::clicked, [=]() {filters2(); });
	//connect(buttonMatLab, &QPushButton::clicked, [=]() {MATLAB_Test();});

	//Add the buttons to the layout
	QLayout* layoutButtons = new QVBoxLayout();
	layoutButtons->addWidget(buttonCustomLayout);
	layoutButtons->addWidget(buttonModelViewBasic);
	layoutButtons->addWidget(buttonModelViewIndex);
	layoutButtons->addWidget(buttonExampleDataReading);
	layoutButtons->addWidget(buttonGearSim);
	layoutButtons->addWidget(buttonMatLab);
	layoutButtons->addWidget(buttonWireFrame);
	layoutButtons->addWidget(buttonJsonPager);

	layoutButtons->addWidget(buttonFilters2);
	layoutButtons->addWidget(buttonProgress);
	this->setLayout(layoutButtons);

	//Set title of window
	setWindowTitle(tr("Example Browser"));
}

void ExampleBrowser::GearSim() {
	PrimaryWindow* window;
	window = new PrimaryWindow();
	window->showMaximized();
}

QString ExampleBrowser::returnStyleSheet(QString fname) {
	//QFile file(QDir::currentPath().append("/").append(fname));
	QFile file(fname);
	file.open(QFile::ReadOnly);
	QString StyleSheet = QLatin1String(file.readAll());
	file.close();

	return StyleSheet;
}

void ExampleBrowser::wireFrame() {
	MainWindow* mainW = new MainWindow(nullptr);
	SplashPage* splash = new SplashPage(mainW);
	mainW->show();
}

void ExampleBrowser::jsonPager() {
	QByteArray jsonText =
		"{\
		\"category1\":{\
			\"field1\":null,\
			\"field2\":2,\
			\"object\":{\
				\"subfield1\":1,\
				\"subfield2\":2,\
				\"array2\":{\
					\"properties\": [\"Dry\", \"Medium\", \"Poor\"],\
					\"data\":[1,2,3]\
				}\
			},\
			\"array1\":[1,2,3]\
		},\
		\"category2\":{\
			\"field1\":1,\
			\"field2\":2,\
			\"array3\":[[1,1],[2,2],3]\
		}\
	}";
	QJsonDocument doc = QJsonDocument::fromJson(jsonText);
	QJsonObject obj = doc.object();
	JsonPager* pager = new JsonPager(obj);
	pager->show();
}

void ExampleBrowser::filters2() {
	//Read the JSON file
	QByteArray val;
	QFile file;

	file.setFileName(":/data/QtGearSim/filters.json");
	file.open(QIODevice::ReadOnly | QIODevice::Text);
	val = file.readAll();
	file.close();
	qDebug() << val;
	QJsonDocument doc = QJsonDocument::fromJson(val);

	//Feed the model and set proxies
	QJsonModel* model = new QJsonModel();
	TableProxyModel* TablesProxy = new TableProxyModel();
	TablesProxy->setSourceModel(model);
	NonTableProxyModel* NonTablesProxy = new NonTableProxyModel();
	NonTablesProxy->setSourceModel(model);

	// i have accidentally trampled the original example,
	// despite attempting specifically to not do that
	// i tried to make mine filters2 and it merged them anyways, idk
	// i don't feel like fixing this right now.
	// whoever wrote it can fix it faster than i can, i think
	// for now i just commented whatever intefered with what i had set up
	// if someone feels like fixing this please make a new filters()
	// - Oleg
	/*
	//view construction
	QTreeView* viewOriginal = new QTreeView();
	QTableView* tableView = new QTableView();
	QJsonModel* model = new QJsonModel(viewOriginal);
	tableView->setModel(model);

	//Tables proxy fill
	TableProxyModel* TablesProxy = new TableProxyModel(viewOriginal);
	TablesProxy->setSourceModel(model);
	
	//Variables proxy fill
	NonTableProxyModel* NonTablesProxy = new NonTableProxyModel(viewOriginal);
	NonTablesProxy->setSourceModel(model);
	*/

	model->loadJson(doc);

	QTreeView* viewOriginal = new QTreeView();
	viewOriginal->setModel(model);
	viewOriginal->expandAll();
	viewOriginal->setStyleSheet("QTreeView::branch {  border-image: url(none.png); }");
	viewOriginal->setRootIsDecorated(false);
	viewOriginal->setItemsExpandable(false);
	viewOriginal->setHeaderHidden(true);
	viewOriginal->header()->resizeSections(QHeaderView::ResizeToContents);

	QTreeView* viewNonTables = new QTreeView();
	viewNonTables->setModel(TablesProxy);
	viewNonTables->expandAll();
	viewNonTables->setStyleSheet("QTreeView::branch {  border-image: url(none.png); }");
	viewNonTables->setRootIsDecorated(false);
	viewNonTables->setItemsExpandable(false);
	viewNonTables->setHeaderHidden(true);
	viewNonTables->header()->resizeSections(QHeaderView::ResizeToContents);

	QTreeView* viewTables = new QTreeView();
	viewTables->setModel(NonTablesProxy);
	viewTables->expandAll();
	viewTables->setStyleSheet("QTreeView::branch {  border-image: url(none.png); }");
	viewTables->setRootIsDecorated(false);
	viewTables->setItemsExpandable(false);
	viewTables->setHeaderHidden(true);
	viewTables->header()->resizeSections(QHeaderView::ResizeToContents);

	QSplitter* split = new QSplitter();
	split->addWidget(viewOriginal);
	split->addWidget(viewNonTables);
	split->addWidget(viewTables);
	split->show();
}

void ExampleBrowser::progress() {

	ProgressLabel* progressLabel1 = new ProgressLabel();
	ProgressLabel* progressLabel2 = new ProgressLabel();

	QSplitter* splitter = new QSplitter();
	splitter->addWidget(progressLabel1);
	splitter->addWidget(progressLabel2);

	progressLabel1->testProgress();

	QTimer* timer = new QTimer(this);
	timer->setSingleShot(true);
	connect(timer, &QTimer::timeout, [=]() {
		progressLabel2->testProgress();
	});
	timer->start(1500);

	splitter->show();
}
