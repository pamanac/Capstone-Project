#include "ResultsPage.h"

ResultsPage::~ResultsPage() {
	delete plotBack;
	delete plotNext;
	delete vLayoutPlots;
	//delete hLayout; //Owned by vLayout
	delete vLayout;
	//delete hLayoutPlotButtons; //Ownerd by vLayoutPlots
	delete selectionsView;
	delete model;
	delete proxy;
}

ResultsPage::ResultsPage(MainWindow* parent)
	: GenericPage("Back", "Home", parent), group(parent->getStack())
{
	//Initialize the buttons
	this->setObjectName("Results page");
	plotBack = new QPushButton("Previous Plot");
	plotNext = new QPushButton("Next Plot");
	
	//Initialize the layouts
	vLayoutPlots = new QVBoxLayout();
	hLayout = new QHBoxLayout();
	vLayout = new QVBoxLayout();
	hLayoutPlotButtons = new QHBoxLayout();

	//Initialize the other objects
	plotStack = new QStackedWidget();
	model = new QJsonModel(this);
	proxy = new FilterProxyModel();
	selectionsView = new QTreeView();

	//Load the JSON model and its proxy with items.
	QByteArray jsonText = PLACEHOLDER_SELECTIONS_TEXT;
	QJsonDocument doc = QJsonDocument::fromJson(jsonText);
	model->loadJson(doc);
	proxy->setSourceModel(model);

	//Populate the view with the aforementioned JSON model.
	selectionsView->setModel(proxy);

	//Configure the settings of the selectionsView.
	selectionsView->expandAll();
	selectionsView->setStyleSheet("QTreeView::branch {  border-image: url(none.png); }");
	selectionsView->setRootIsDecorated(false);
	selectionsView->setItemsExpandable(false);
	selectionsView->setHeaderHidden(true);
	selectionsView->header()->resizeSections(QHeaderView::ResizeToContents);
	selectionsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	//Build the labels that make up the placeholder graphs.
	QLabel* label1 = new QLabel(this);
	QPixmap px1 = QPixmap(":/data/Resources/plot1.png");
	label1->setPixmap(px1);
	label1->setMask(px1.mask());

	QLabel* label2 = new QLabel(this);
	QPixmap px2 = QPixmap(":/data/Resources/plot2.png");
	label2->setPixmap(px2);
	label2->setMask(px2.mask());

	QLabel* label3 = new QLabel(this);
	QPixmap px3 = QPixmap(":/data/Resources/plot3.png");
	label3->setPixmap(px3);
	label3->setMask(px3.mask());

	//Add aforementioned labels to the stack of plots.
	plotStack->addWidget(label1);
	plotStack->addWidget(label2);
	plotStack->addWidget(label3);

	//Addd the buttons to the relevant layouts
	hLayoutPlotButtons->addWidget(plotBack, 1, Qt::AlignLeft);
	hLayoutPlotButtons->addWidget(plotNext, 1, Qt::AlignRight);

	//Add the graphs, and buttons related to them, to the vertical layout
	vLayoutPlots->addWidget(plotStack, 1);
	vLayoutPlots->addLayout(hLayoutPlotButtons, 0);

	//Horizontally arrange the selectionsView and the graphs
	hLayout->addWidget(selectionsView, 1);
	hLayout->addLayout(vLayoutPlots, 1);

	//Finalize the window layout to show our contents
	vLayout->addLayout(hLayout);
	vLayout->addLayout(getLayout());
	//vLayout->addLayout(hLayoutWindowButtons);
	setLayout(vLayout);

	//Connect the plot buttons to the plot stack navigation.
	connect(plotBack, &QPushButton::clicked, [=]() {
		int idx = plotStack->currentIndex();
		int min = 0;
		if (idx > min) {
			plotStack->setCurrentIndex(idx - 1);
		}
	});
	connect(plotNext, &QPushButton::clicked, [=]() {
		int idx = plotStack->currentIndex();
		int max = plotStack->count();
		if (idx < max) {
			plotStack->setCurrentIndex(idx + 1);
		}
	});

};

void ResultsPage::nextPage() {
	setIndex(0);
}
