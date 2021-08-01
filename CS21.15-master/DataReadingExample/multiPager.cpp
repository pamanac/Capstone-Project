#include "multiPager.h"
#include <QGraphicsScene>

multiPager::~multiPager() {
	//delete deleg; Never actually used
	delete dmodel;
	delete dview;
	delete stack;
	delete next;
	delete save;
	delete workingFile;
	delete layout;
}

multiPager::multiPager(QWidget* parent) : QWidget(parent) {
	layout = new QVBoxLayout();
	setLayout(layout);
	init(nullptr);
}

multiPager::multiPager(GearSimDataFile* file, QWidget* parent): QWidget(parent) {
	layout = new QVBoxLayout();
	setLayout(layout);
	init(file);
}

bool multiPager::setFile(const QString& fileName) {
	GearSimDataFile* file = new GearSimDataFile(fileName);
	return setFile(file);
}

bool multiPager::setFile(GearSimDataFile* file) {
	if (file->isValidFile()) {
		workingFile = file;
		
		layout->removeWidget(stack);
		layout->removeWidget(next);
		layout->removeWidget(save);
		init(file); //TODO: VERY POSSIBLY A BIG MEMORY LEAK
		return true;
	}
	return false;
}

void multiPager::init(GearSimDataFile* file) {
	workingFile = file;

	stack = new QStackedWidget();
	tables = file->getDataTablesList();
	for (GearSimDataTable* table : tables) {
		tableModel* model = new tableModel(this, table);
		tmodels.append(model);
	}
	SDelegate* delegate = new SDelegate;
	for (tableModel* model : tmodels) {
		QTableView* view = new QTableView(this);
		view->setModel(model);
		view->setItemDelegate(delegate);
		tviews.append(view);
	}

	data = file->getVariablesList();
	dmodel = new TreeModel(data, this);
	dview = new QTreeView(this);
	dview->setItemDelegateForColumn(2, delegate);
	SDelegate* del = new SDelegate();
	dview->setModel(dmodel);

	//layout = new QVBoxLayout();
	stack->addWidget(dview);
	for (QTableView* v : tviews)
		stack->addWidget(v);

	layout->addWidget(stack);

	next = new QPushButton();
	connect(this->next, &QPushButton::clicked, [=]() {
		int nextIndex = stack->currentIndex() + 1;
		if (nextIndex >= stack->count())
			nextIndex = 0;
		QString nextButtonName = QString::fromStdString("Next Page (" + std::to_string((nextIndex + 1)) + "/" + std::to_string((stack->count())) + ")");
		qDebug() << nextButtonName;
		next->setText(nextButtonName);
		stack->setCurrentIndex(nextIndex);
	});

	save = new QPushButton("Save to File");
	connect(this->save, &QPushButton::clicked, [=]() {file->writeToDisk();});
	layout->addWidget(save);
	layout->addWidget(next);
	//setLayout(layout);

	if (!tables.isEmpty()) {
		for (int i = 0; i < tables.count(); i++) {
			QStringList colNames = tables[i]->getColumnNames();
			if (colNames.count() == 0) {
				QChartView* chart = tables[i]->createGraph();
				if (chart) {
					stack->addWidget(chart);
				}
			}
		}
	}

	next->setText(QString::fromStdString("Next Page (1/" + std::to_string((stack->count())) + ")"));
}