#include "DataEntryPage.h"
#include "ResultsPage.h"
#include "TableProxyModel.h"
#include "NonTableProxyModel.h"
#include "TabProxyModel.h"

DataEntryPage::~DataEntryPage() {
	//delete group; //We do not take ownership of group.
	delete horiz;
	delete vert;
	delete testTypes;
	delete aircraft;
	delete pager;
	delete componentsView;
	delete filemodel;
	delete tabs;
}

DataEntryPage::DataEntryPage(MainWindow* parent) :
	GenericPage("Back", "Simulate", parent), group(parent->getStack())
{
	moduleLoader = ModuleLoader((parent->programState).userSelections);
	proxyList = QList<QAbstractProxyModel*>();
	viewStack = new QStackedWidget(this);
	QTreeView* testView = new QTreeView();
	testView->setModel(moduleLoader.model);
	//testView->show();

	this->testTypes = new QComboBox();
	this->setObjectName("Data entry page");
	horiz = new QHBoxLayout();
	vert = new QVBoxLayout();
	pager = nullptr;
	
	//Create the primary tabs using the children of the root
	QList<QString>* primaryTabs = new QList<QString>;
	QJsonTreeItem* root = moduleLoader.model->item(QModelIndex());
	for (int i = 0, max = root->childCount(); i < max; i++) {
		QString key = root->child(i)->key();
		primaryTabs->append(key);
	}
	// 
	// QList<QString> tabNames = { "Drop Aircraft", "Rigid Leg", "Scenario Attributes" };
	// for (QString name : tabNames) {
	//		primaryTabs->append(name);
	// }

	tabs = new NestedTabBar(primaryTabs);
	auto currentChangedSlot = [=](QString primaryText, QString secondaryText) {
		tabs->setCurrentWidgetOf(viewStack);
	};
	tabs->setConnections(currentChangedSlot);

	//Create the secondary row of tabs for navigating using the children of the children of the root.
	//also make the proxies, views
	for (int i = 0, imax = root->childCount(); i < imax; i++) {
		QString primaryKey = root->child(i)->key();
		for (int j = 0, jmax = root->child(i)->childCount(); j < jmax; j++) {
			QString secondaryKey = root->child(i)->child(j)->key();
			tabs->addTab(primaryKey, secondaryKey);

			TabProxyModel* proxyModel = new TabProxyModel(primaryKey, secondaryKey, this);	
			proxyModel->setSourceModel(moduleLoader.model);
			proxyList.append(proxyModel);
			QTreeView* view = new QTreeView();
			view->setObjectName(primaryKey + secondaryKey);
			view->setModel(proxyModel);
			view->expandAll();
			view->setStyleSheet("QTreeView::branch {  border-image: url(none.png); }");
			view->setRootIsDecorated(false);
			view->setItemsExpandable(false);
			view->setHeaderHidden(true);
			view->header()->resizeSections(QHeaderView::ResizeToContents);
			viewStack->addWidget(view);
		}
	}
	vert->addWidget(tabs, 0);
	vert->addWidget(viewStack, 0);
	//Build the file structure from which we load data.
	filemodel = new QStandardItemModel();
	QString commonPath = "C:\\ProgramData\\GearSim-2.8\\USER\\Default\\";
	QStringList subPaths = { "AIRCRAFT", "DIABLO", "FCS", "LEG", "OLEO", "RUNWAY", "SINGLE", "TIRE" };
	QStringList datFiles;

	//Obtain pathing for all of the files that will appear in componentsView.
	for (QString dir : subPaths) {
		QDir directory(commonPath + dir);
		QStringList dirFiles = directory.entryList(QStringList() << "*.DAT" << "*.dat", QDir::Files);
		dirFiles.prepend(dir + "\\");
		QStringList composedNames;
		for (QString item : dirFiles) {
			item = dir + "\\" + item;
			composedNames.append(item);
		}
		datFiles = datFiles + composedNames;
	}

	//Fill filemodel with the absolute path of each file that will be loadable.
	for (QString filename : datFiles) {
		filemodel->appendRow(new QStandardItem(filename));
	}

	//Add the list of aircraft files to the window.
	componentsView = new QListView();
	componentsView->setModel(filemodel);
	horiz->addWidget(componentsView);

	//Add the aircraft placeholder to the window.
	aircraft = new QLabel();
	aircraft->setPixmap(QPixmap(":/data/Resources/Narrobody_Commercial.PNG"));
	horiz->addWidget(aircraft);

	//Connect file selection changes to new multipager construction
	connect(componentsView->selectionModel(), &QItemSelectionModel::selectionChanged, [=](const QItemSelection& selected) {
		for (const auto& index : selected.indexes()) {
			bool previousPagerExists = pager;

			//Construct the new pager
			multiPager* newPager = new multiPager(new GearSimDataFile(commonPath + filemodel->data(index).toString()));

			//Attempt to catch any nullptr/invalid address issues.
			if (previousPagerExists) {

				//Replace the old pager with a new one.
				QLayoutItem* replaceResult = vert->replaceWidget(pager, newPager);

				//Deallocate the old pager from heap if the replace succeeded.  Also deallocate the operation result.
				if (replaceResult) {
					delete replaceResult;
					//delete pager;
				}

				//Replace the pager pointer with the newPager pointer
				pager = newPager;

			} else { //This previouspager not existing case exists only to catch errors and should be a rare occurance.

				//We don't have a page for some reason, so replace the pager pointer 
				pager = newPager;
				
				//To show the pager, it must be added to widgets.
				vert->addWidget(pager, 1);
			}

		}
	});

	//Initial pager construction/Default page to start on.
	pager = new multiPager(new GearSimDataFile("C:\\ProgramData\\GearSim-2.8\\USER\\Default\\AIRCRAFT\\AIRCRAFT.DAT"));
	
	//Combine the layouts together.
	vert->addLayout(horiz);
	vert->addWidget(pager, 1);
	vert->addLayout(getLayout()); //Add the generic page's layout
	setLayout(vert);
}

/*
* We do not need to override backpage, since we don't do anything different.
void DataEntryPage::backPage() {
	//group->setCurrentIndex(group->currentIndex() - 1);
}
*/

void DataEntryPage::nextPage() {
	//If we are are the maximum index, create the results page.
	if (group->count() - 1 == group->currentIndex()) {
		//MainWindow* localParent = static_cast<MainWindow*>(parent());
		group->addWidget(new ResultsPage(localParent));
	}

	//Go to the next page.
	setIndex(group->currentIndex() + 1);
}