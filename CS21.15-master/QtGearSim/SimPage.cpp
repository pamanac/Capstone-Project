#include "SimPage.h"
#include "DataEntryPage.h"
#include "TextDelegate.h"

SimPage::~SimPage() {
	//delete group; //We do not take ownership of group.
	delete gridLayout;
	delete labelDirectory;
	delete labelAircraftSelected;
	delete simulationsList;
	delete simulationsOptionsList;
	delete simulationsModel;
	delete optionProxy;
}

SimPage::SimPage(QString aircraftSelectedName, QString directorySelectedName, MainWindow* parent) :
	GenericPage(parent)
{
	this->group = parent->getStack();
	this->setObjectName("Sim page");
	this->setParent(parent);

	//Init member variables
	labelDirectory = new QLabel(directorySelectedName);
	labelAircraftSelected = new QLabel(aircraftSelectedName);
	simulationsList = new QListView();
	simulationsOptionsList = new QListView();
	optionProxy = new QSortFilterProxyModel();
	gridLayout = new QGridLayout();
	simulationsModel = new QStandardItemModel();
	simulationsOptionsModel = new QStandardItemModel();
	labelTestDescription = new QLabel();
	TextDelegate* del = new TextDelegate();

	QStringList additionalOptions = { "Flexible Leg [DROP TEST]", "AdditionalOption 2 [BRAKE TEST]", "AdditionalOption 3 [LANDING TEST]", "AdditionalOption [BRAKE TEST]" };
	for (QString option : additionalOptions)
		simulationsOptionsModel->appendRow(new QStandardItem(option));

	optionProxy->setSourceModel(simulationsOptionsModel);
	optionProxy->setFilterRegularExpression("DROP TEST");

	//Fill the list models
	simulationsModel->appendRow(new QStandardItem("Brake Test"));
	simulationsModel->appendRow(new QStandardItem("Drop Test"));
	simulationsModel->appendRow(new QStandardItem("Landing Test"));

	//Fill the list views
	simulationsList->setModel(simulationsModel);
	simulationsOptionsList->setModel(optionProxy);
	simulationsOptionsList->setItemDelegate(del);
	for(int row = 0; row < optionProxy->rowCount(); row++)
		simulationsOptionsList->openPersistentEditor(optionProxy->index(row,0));

	//TODO: link the checkbox items to the programState.

	// connect view selection to program state
	connect(simulationsList->selectionModel(), &QItemSelectionModel::selectionChanged, [=](const QItemSelection& selected) {
		QString oldSelection = parent->programState.userSelections.simulation;
		for (const auto& index : selected.indexes()) {
			QString simulationSelected = simulationsModel->data(index).toString();
			labelTestDescription->setText(simulationSelected + " description: "
				+ "Placeholder description.  The user will be able to read about the selected simulation option's purpose and general idea/theory here.  Ideally, it will be accompanied by a video or image to help convey the same ideas as this description would."
			); // Must update individual descriptions 
			QString regEx = simulationSelected.toUpper();

			qDebug() << regEx;
			optionProxy->setFilterRegularExpression(regEx);
			for (int row = 0; row < optionProxy->rowCount(); row++)
				simulationsOptionsList->openPersistentEditor(optionProxy->index(row, 0));
			parent->programState.userSelections.simulation = simulationSelected;
			qDebug() << "programState.userSelections.simulation = " << simulationSelected;
		}
		
		//If selection is different, remove all pages ahead of us.
		if (parent->programState.userSelections.simulation != oldSelection) {
			//Remove all pages after this.
			int count = group->count();
			for (int index = group->count(); index > group->currentIndex(); index--) {
				QWidget* del = group->widget(index);
				group->removeWidget(del);
				delete del;
			}
		}
	});

	//Add items to grid layout at (r, c)
	gridLayout->addWidget(labelDirectory, 0, 0, Qt::AlignmentFlag::AlignLeft);
	gridLayout->addWidget(labelAircraftSelected, 0, 0, Qt::AlignmentFlag::AlignRight);
	gridLayout->addWidget(simulationsList, 1, 0, Qt::AlignmentFlag::AlignLeft);
	gridLayout->addWidget(simulationsOptionsList, 1, 0, Qt::AlignmentFlag::AlignRight);
	labelTestDescription->setMaximumWidth(250);
	labelTestDescription->setWordWrap(true);
	gridLayout->addWidget(labelTestDescription, 1, 0, Qt::AlignmentFlag::AlignCenter);
	gridLayout->addLayout(getLayout(), 2, 0); //Add the generic page buttons.
	setLayout(gridLayout);
};

void SimPage::nextPage() {
	//Create the next page if it doesn't exist yet.
	if (group->currentIndex() == group->count() - 1) {
		//MainWindow* localParent = static_cast<MainWindow*>(parent());
		group->addWidget(new DataEntryPage(localParent));
	}

	//Cycle to the next page.
	setIndex(group->currentIndex() + 1);
};