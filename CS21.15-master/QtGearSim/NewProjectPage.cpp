#include "NewProjectPage.h"
#include "qjsonmodel.h"
#include "DataEntryPage.h"
#include "ExampleBrowser.h"
#include "SimPage.h"
#include "MainWindow.h"

NewProjectPage::NewProjectPage(MainWindow* parent)
	: GenericPage(parent)
{
	previousAircraftSelected = "";
	this->setObjectName("Aircraft page");
	this->group = localParent->getStack();
	aircraftTypesView = new QListView(this);
	layoutHoriz = new QHBoxLayout();
	layoutVert = new QVBoxLayout();
	aircraftDescription = new QLabel();
	aircraftDescription->setWordWrap(true);
	aircraftDescription->setObjectName("AirDescription");
	//aircraftDescription->setMaximumWidth(200);

	modelPreviewText = new QLabel("Model Preview:");
	narrobodyPreview = new QLabel();
	narrobodyPreview->setPixmap(QPixmap(":/data/Resources/Narrobody_Commercial.PNG"));
	militaryIpreview = new QLabel();
	militaryIpreview->setPixmap(
		QPixmap(":/data/Resources/MilitaryI.jpg").scaled(narrobodyPreview->width(), narrobodyPreview->height(), Qt::KeepAspectRatio));
	militaryIIpreview = new QLabel();
	militaryIIpreview->setPixmap(QPixmap(":/data/Resources/MilitaryII.jpg").scaled(narrobodyPreview->width(), narrobodyPreview->height(), Qt::KeepAspectRatio));
	aircraftPreviewStack = new QStackedWidget();
	aircraftPreviewStack->addWidget(narrobodyPreview);
	aircraftPreviewStack->addWidget(militaryIpreview);
	aircraftPreviewStack->addWidget(militaryIIpreview);

	globalHorizontal = new QHBoxLayout();
	previewLayout = new QVBoxLayout();
	previewLayout->addWidget(modelPreviewText);
	previewLayout->addWidget(aircraftPreviewStack);
	previewLayout->addWidget(aircraftDescription);
	layoutVert->addWidget(aircraftTypesView);
	globalHorizontal->addLayout(layoutVert);
	globalHorizontal->addLayout(previewLayout);
	aircraftModel = new QStandardItemModel();
	QStringList aircraftNames = { "Drop Aircraft", "Symmetric Aircraft", "Narrowbody Commercial", "Military Type I", "Military Type II" };
	for (QString airtype : aircraftNames)
		aircraftModel->appendRow(new QStandardItem(airtype));
	aircraftTypesView->setModel(aircraftModel);
	aircraftTypesView->setObjectName("AircraftsView");
	connect(aircraftTypesView->selectionModel(), &QItemSelectionModel::selectionChanged, [=](const QItemSelection& selected) {
		for (const auto& index : selected.indexes()) {
			QString aircraftSelected = aircraftModel->data(index).toString();
			parent->programState.userSelections.airplane = aircraftSelected;
			qDebug() << "programState.userSelections.airplane = " << aircraftSelected;
			// is there a better way to do the logic below this comment?
			aircraftDescription->setText(aircraftSelected + ": " 
				+ "Placeholder description.  The user will be able to read about the selected aircraft type's purpose and general idea/theories here.  Ideally, it will be accompanied by a video or image to help convey the same ideas as this description would."
			); // Must put individual descriptions for each aircraft in if statements
			if (aircraftSelected == "Drop Aircraft")
				aircraftPreviewStack->setCurrentIndex(0);
			else if (aircraftSelected == "Symmetric Aircraft")
				aircraftPreviewStack->setCurrentIndex(1);
			else if (aircraftSelected == "Narrowbody Commercial")
				aircraftPreviewStack->setCurrentIndex(2);
			else if (aircraftSelected == "Military Type I")
				aircraftPreviewStack->setCurrentIndex(3);
			else if (aircraftSelected == "Military Type II")
				aircraftPreviewStack->setCurrentIndex(4);
		}
		});
	
	previewLayout->addLayout(getLayout());
	setLayout(globalHorizontal);
	this->setStyleSheet(ExampleBrowser::returnStyleSheet(":/data/Resources/NewProjectPage.qss"));
}
void NewProjectPage::backPage() {
	setIndex(group->currentIndex() - 1);
}

//TODO: change the logic for deleteing future pages to use the new userselections type.
void NewProjectPage::nextPage() {
	QString aircraftSelected = aircraftTypesView->currentIndex().data(Qt::DisplayRole).toString();
	if (aircraftSelected.length() == 0) {
		qDebug() << "Aircraft selection is empty!";
		//throw std::invalid_argument("Aircraft selection empty");
		aircraftSelected = aircraftTypesView->rootIndex().data().toString();
	}
	QString directorySelected = "C:\\ProgramData\\GearSim-2.8\\";

	if (previousAircraftSelected.length() != 0) { //Page was previously created
		if (previousAircraftSelected == aircraftSelected) { //same settings
			setIndex(group->currentIndex() + 1); //Go back, user probably didn't mean to go back
		} else { //not same settings
			previousAircraftSelected = aircraftSelected;
			
			//Remove all pages after this.
			int count = group->count();
			//for (int index = group->currentIndex() + 1; index < count; index++) {
			for (int index = group->count(); index > group->currentIndex(); index--) {
				QWidget* del = group->widget(index);
				group->removeWidget(del);
				delete del;
			}
			
			//MainWindow* localParent = static_cast<MainWindow*>(parent());
			group->addWidget(new SimPage(aircraftSelected, directorySelected, localParent));
			qDebug() << group->currentIndex();
			setIndex(group->currentIndex() + 1);
		}
	} else { //first visit to NewProjectPage
		previousAircraftSelected = aircraftSelected;
		//MainWindow* localParent = static_cast<MainWindow*>(parent());
		//qDebug() << "(GNPP)localParent : " << localParent;
		SimPage* temp = new SimPage(aircraftSelected, directorySelected, localParent);
		qDebug() << "Current index: " << group->currentIndex();
		group->addWidget(temp);
		qDebug() << "Current count: " << group->count();
		qDebug() << "Index of temp: " << group->indexOf(temp);
		setIndex(group->currentIndex() + 1); /* set view to next page */
	}
}

