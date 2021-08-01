#include"GenericPage.h"
#include<qlistview.h>
#include"ExampleBrowser.h"
GenericPage::GenericPage(QString backText, QString forwardText, MainWindow* parent, bool useVerticalButtonFormat)
	: pageGroup(parent->getStack()), localParent(parent) //This had an initializer for parent: QWidget(parent) which broke the mainWindow menu
{
	//Initialize the internal widgets
	buttonBack = new QPushButton(backText);
	buttonNext = new QPushButton(forwardText);
	grid = new QGridLayout;
	this->setStyleSheet(ExampleBrowser::returnStyleSheet(":/data/Resources/GenericPage.qss"));

	//Set the button names
	buttonBack->setObjectName("buttonBack");
	buttonNext->setObjectName("buttonNext");

	//Connect the buttons to the indexing.
	connect(buttonBack, &QPushButton::clicked, [=]() {
		backPage();
	});
	connect(buttonNext, &QPushButton::clicked, [=]() {
		nextPage();
	});

	//Add the buttons to the layout at (row, col).
	if (useVerticalButtonFormat) {
		grid->addWidget(buttonBack, 1, 0);
		grid->addWidget(buttonNext, 0, 0);
	} else {
		grid->addWidget(buttonBack, 0, 0, Qt::AlignmentFlag::AlignLeft);
		grid->addWidget(buttonNext, 0, 0, Qt::AlignmentFlag::AlignRight);
	}

}

void GenericPage::nextPage() {
	setIndex(pageGroup->currentIndex() + 1);
}

void GenericPage::backPage() {
	setIndex(pageGroup->currentIndex() - 1);
}

bool GenericPage::setIndex(int targetIndex) {	
	int finalIndex;
	unsigned int pageCount;
	
	//Validate the pageGroup exists.
	if (!pageGroup) {
		return false;
	}
	pageCount = pageGroup->count();

	//Evaluate the index validity.
	if (targetIndex >= pageCount) { //Handle case of index being too large
		pageGroup->setCurrentIndex(pageCount - 1);
		updateParentDockSelection();
		return false;
	}
	
	//Given a negative index, use Python style reverse indexing.
	if (targetIndex < 0) {
		//Handle the case of an index that is too far into the negatives
		if (abs(targetIndex) > pageGroup->count()) {
			pageGroup->setCurrentIndex(0);
			updateParentDockSelection();
			return false;
		}

		pageGroup->setCurrentIndex(pageCount + targetIndex);
		updateParentDockSelection();
		return true;
	}

	//Handle case of index being acceptable
	pageGroup->setCurrentIndex(targetIndex);
	updateParentDockSelection();

	return true;
}

bool GenericPage::updateParentDockSelection() {
	//verify pageGroup exists.
	if (!pageGroup) {
		return false;
	}
	
	//Fetch a reference to the dock.
	QListView* mainWindowDock = static_cast<QListView*>(localParent->getDock()->widget());

	//Update the nav dock page's selection using the current index of the pageGroup.
	mainWindowDock->setCurrentIndex(mainWindowDock->model()->index(pageGroup->currentIndex(), 0));
}


/*
const QModelIndex GenericPage::getDockIndexAtIndex(int targetIndex) {
	QListView* mainWindowDock = static_cast<QListView*>(localParent->getDock()->widget());
	
};
*/
QLayout* GenericPage::getLayout() {
	return grid;
}

GenericPage::GenericPage(MainWindow* parent) : GenericPage("Back", "Next", parent) {
	//Intended to be empty
}

GenericPage::~GenericPage() {
	delete buttonBack;
	delete buttonNext;
	//delete grid; //We assume that grid is taken ownership of by child objects. Failure to make this assumption results in nullptr delete crashes.
}