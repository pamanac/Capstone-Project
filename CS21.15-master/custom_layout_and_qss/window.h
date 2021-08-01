#pragma once
#include <QtWidgets>
#include <QtCore>

class MyWindow : public QWidget {
	//Q_OBJECT
	public:
		MyWindow();
	public slots: //Slots are registered by Qt for event handling.
		void storeContent(QString fileName);
		void showFilePath();
		void toggleTheme();
	private:
		QLabel* createLabel(const QString& text);
		QString returnStyleSheet(QString fName);

		QPushButton* button_StoreContent;
		QPushButton* button_PathContent;
		QPushButton* button_ToggleTheme;
	
		QTabWidget* tabPri;
		QTabWidget* tabSec;

		QString styleSheet;
};