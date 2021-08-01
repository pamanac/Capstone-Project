#include "SplashPage.h"
#include "NewProjectPage.h"
#include <QFileDialog>
#include "MainWindow.h"
SplashPage::~SplashPage() {
	//delete group; //We do not take ownership of group.
	delete layout;
	delete version;
	delete logo;
	delete mainHeader;
	delete subHeader;
	delete newButton;
	delete loadButton;
}

SplashPage::SplashPage(MainWindow* parent) : GenericPage("Load Project", "New Project", parent, true) {
	this->setObjectName("Splash page");
	this->group = parent->getStack();
	layout = new QVBoxLayout(this);
	logo = new QLabel(this);
	logo->setObjectName("GearSimLogo");
	
	//Create the labels
	mainHeader = new QLabel(this);
	subHeader = new QLabel(this);
	version = new QLabel(this);

	//Set the label items.
	mainHeader->setObjectName("H1");
	subHeader->setObjectName("H2");
	logo->setPixmap(QPixmap(":/data/Resources/gearsim.png"));
	mainHeader->setText("Landing Gear Dynamics, Loads and Systems Analysis");
	subHeader->setText("SDI Engineering, Inc.");
	QString versionID = QString::fromStdString(std::to_string(this->getVersionID()));
	versionID.truncate(3);
	version->setText("Version " + versionID );
	version->setObjectName("version");

	//Add all the widgets to the layout
	layout->addWidget(logo);
	layout->addWidget(mainHeader);
	layout->addWidget(subHeader);
	layout->setAlignment(Qt::AlignCenter);
	
	//Add the generic page layout for the next/back buttons
	layout->addLayout(getLayout());

	//Add the version number below the buttons
	layout->addWidget(version);
	layout->setObjectName("SplashLayout");

	//Apply the layout to the widget.
	setLayout(layout);
}

void SplashPage::nextPage(){
	if (this->group->count() <= 1) {
		qDebug() << "Splash actualparent after press new: " << this->parent();
		qDebug() << "Splash Localparent after press new: " << localParent;
		this->group->addWidget(new NewProjectPage(localParent));
	}
	setIndex(this->group->currentIndex() + 1);
};

void SplashPage::backPage(){
	QFileDialog dialog(this);
	QString filename = dialog.getOpenFileName(this, tr("Open Project"), QDir::currentPath(), tr("GearSim Project (*.grsim)"));
};