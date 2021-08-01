#ifndef GENERIC_NEW_PROJECT_PAGE_H
#define GENERIC_NEW_PROJECT_PAGE_H

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
#include <QComboBox>
#include <QStandardItemModel>

#include "QJsonModel.h"
#include "GenericPage.h"

class NewProjectPage : public GenericPage {
	Q_OBJECT
private:
	QListView* aircraftTypesView;
	QStackedWidget* group;
	QHBoxLayout* layoutHoriz;
	QVBoxLayout* layoutVert;
	QStandardItemModel* aircraftModel;
	QLabel* narrobodyPreview, * militaryIpreview, * militaryIIpreview;
	QLabel* modelPreviewText;
	QVBoxLayout* previewLayout;
	QStackedWidget* aircraftPreviewStack;
	QLabel* aircraftDescription;
	QHBoxLayout* globalHorizontal;

	QString previousAircraftSelected;
public:
	NewProjectPage(MainWindow* parent);

	void backPage() override;
	void nextPage() override;
};

#endif