#include "borderlayout.h"
#include "window.h"
#include <QtWidgets>

//Now deprecated since it crashes the app.
void MyWindow::showFilePath() {
	//Shows the file path output via the storeContent button.  Resizes this button to show the full length.
	qDebug() << "... show file path";
	QString output(QDir::currentPath());
	int xSize = 12 * output.length();
	int ySize = button_PathContent->iconSize().height() * 2;
	
	//button_PathContent->setText(output);
	//button_PathContent->resize(QSize(xSize, ySize));
}

QString MyWindow::returnStyleSheet(QString fName) {
	QFile File(QDir::currentPath().append('/').append(fName));
	File.open(QFile::ReadOnly);
	QString StyleSheet = QLatin1String(File.readAll());
	File.close();
	return StyleSheet;
}

void MyWindow::toggleTheme() {
	qDebug() << "... toggleTheme";

	if (styleSheet == "") {
		styleSheet = "testStyle1.qss";
		qDebug() << "\t Current theme number: 0/4";
		qDebug() << "\t Future theme number:  1";
	} else {
		unsigned int currStyle = (styleSheet.at(9)).toLatin1(); //requires the testStyle<number> names.  All others seg fault.
		currStyle -= 48; //Remove ASCII from the conversion.
		qDebug() << "\t Last theme number: " << currStyle;
		qDebug() << "\t New theme number:  " << currStyle + 1;

		//Determine style to use.
		switch (currStyle) {
			case 1:
				styleSheet = "testStyle2.qss"; //dark theme https://github.com/EClaesson/QTDark
				break;
			case 2:
				styleSheet = "testStyle3.qss"; //Dark solarized https://gist.github.com/Zren/e91ad5197f9d6b6d410f
				break;
			default:
				styleSheet = "";
				break;
		};
	};

	//Apply sheet to the qApp.
	qApp->setStyleSheet(returnStyleSheet(styleSheet));
}

//void window::storeContent()
void MyWindow::storeContent(QString fileName) {
	qDebug() << "... store content";
	//QString fileName = "HelloWorld.txt";
	QString message("Hello World!  I HATE LAMBDAS USING 1 LESS VARIABLE");
	
	//Output to file at current working directory.
	QFile file(QDir::currentPath().append('/').append(fileName));
	if (!file.open(QIODevice::WriteOnly)) {
		qWarning() << "Can not open file with write access";
		return;
	}
	//button_StoreContent->setText("Sent");
	QTextStream stream(&file);
	stream << message;
}

QLabel* MyWindow::createLabel(const QString& text) {
	QLabel* label = new QLabel(text);
	label->setFrameStyle(QFrame::Box | QFrame::Raised);
	return label;
}

MyWindow::MyWindow() {
	//Create the buttons
	QWidget* widget_buttonsGroup = new QWidget();
	QWidget* widget_buttonsGroup2 = new QWidget();
	QPushButton* button_StoreContent = new QPushButton("Store Content", widget_buttonsGroup);
	QPushButton* button_PathContent = new QPushButton("Show Path", widget_buttonsGroup);
	QPushButton* button_ToggleTheme = new QPushButton("Toggle Theme", widget_buttonsGroup);
	qDebug() << button_StoreContent->geometry().topLeft();
	qDebug() << button_PathContent->geometry().topLeft();
	qDebug() << button_ToggleTheme->geometry().topLeft();

	//Register the buttons to window functions.
	//connect(button_PathContent, &QPushButton::clicked, this, &MyWindow::showFilePath); //will crash
	connect(button_StoreContent, &QPushButton::clicked, [this]() {this->storeContent("HelloWorld.txt"); });
	connect(button_ToggleTheme, &QPushButton::clicked, this, &MyWindow::toggleTheme);

	//Button set
	QLayout* layout_Buttons = new QHBoxLayout();
	layout_Buttons->addWidget(button_StoreContent);
	layout_Buttons->addWidget(button_PathContent);
	layout_Buttons->addWidget(button_ToggleTheme);
	widget_buttonsGroup->setLayout(layout_Buttons);

	//tab set
	QWidget* borderlayoutTest = new QWidget();
	QWidget* borderlayoutTest2 = new QWidget();
	QHBoxLayout* layout_tab = new QHBoxLayout();
	QHBoxLayout* layout_tab2 = new QHBoxLayout();
	tabPri = new QTabWidget();
	tabSec = new QTabWidget();
	//tabPri->addTab(tabSec, "Tab Test");
	tabSec->addTab(borderlayoutTest, "Border Layout");
	tabSec->addTab(widget_buttonsGroup, "Horizontal Buttons");
	tabPri->addTab(new QWidget(), "Mem leak");
	tabPri->addTab(new QWidget(), "Mem leak 2.0");
	layout_tab->addWidget(tabPri, 1, Qt::AlignLeft);
	layout_tab2->addWidget(tabSec);
	tabPri->setMaximumWidth(600);

	//Border set
	QTextBrowser* centralWidget = new QTextBrowser;
	centralWidget->setPlainText(tr("Central widget"));
	BorderLayout* layout = new BorderLayout;
	layout->addWidget(centralWidget, BorderLayout::Center);
	layout->addWidget(createLabel("North"), BorderLayout::North);
	layout->addWidget(createLabel("West"), BorderLayout::West);
	layout->addWidget(createLabel("East 1"), BorderLayout::East);
	layout->addWidget(createLabel("East 2"), BorderLayout::East);
	layout->addWidget(createLabel("South"), BorderLayout::South);
	//layout->addLayout(layout_Buttons);
	//setLayout(layout);
	borderlayoutTest->setLayout(layout);

	setWindowTitle(tr("Border Layout"));

	QVBoxLayout* vspace = new QVBoxLayout(this);
	vspace->addLayout(layout_tab);
	vspace->addLayout(layout_tab2);
	//vspace->addStretch(1);
	vspace->setStretchFactor(layout_tab2, 9999999999);
	//borderlayoutTest->setMaximumHeight(9999);
	//widget_buttonsGroup->setMaximumSize(1,1);
	//widget_buttonsGroup->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
};