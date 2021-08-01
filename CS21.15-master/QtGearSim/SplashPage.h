#ifndef SPLASH_PAGE_H
#define SPLASH_PAGE

#include <QApplication>
#include <QDebug>
#include <QObject>
#include <QWidget>
#include <QStackedWidget>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QLayout>
#include <QPixmap>
#include <QMainWindow>
#include <QListView>
#include "MainWindow.h"
#include "GenericPage.h"

class SplashPage : public GenericPage {
Q_OBJECT
private:
	QPushButton* loadButton;
	QPushButton* newButton;
	QLabel* logo;
	QLabel* mainHeader, *subHeader;
	QLabel* version;
	static const float getVersionID() {
		return 2.80;
	}
	QVBoxLayout* layout;
	QStackedWidget* group;
public:
	SplashPage(MainWindow* parent);
	~SplashPage();
	void nextPage() override;
	void backPage() override;
};

#endif