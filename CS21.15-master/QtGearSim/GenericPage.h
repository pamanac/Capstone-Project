#ifndef GENERIC_PAGE_H
#define GENERIC_PAGE_H

#include <QPushButton>
#include <QStackedWidget>
#include <QGridLayout>
#include "MainWindow.h"

/*
	GenericPage is the core behind the linear paging system.
	It depends on two buttons and a MainWindow, where the buttons will invoke the methods backPage and nextPage.
	
	New windows should inherit from this generic page and override the nextPage and backPage functions.

	Generic page will not take ownership of the MainWindow pointer!
	*/
class GenericPage : public QWidget {
	Q_OBJECT
private:
	QPushButton *buttonBack, *buttonNext;
	QStackedWidget* pageGroup;
	QGridLayout* grid;
	
	/*
	Internal method to update the parent's navigation dock selection.
	Preconditions:
		None
	Postconditions:
		None
	Returns:
		False whenever the index is not found.
		True otherwise.
	*/
	bool updateParentDockSelection();
public:
	MainWindow* localParent;
	GenericPage(MainWindow* parent = nullptr);
	GenericPage(QString backText, QString forwardText, MainWindow* parent = nullptr, bool useVerticalButtonFormat = false);
	virtual ~GenericPage();
	/*
	NextPage increments the pageGroup index.
	This method is called whenever buttonNext is used and is intended to be specialized by child objects.
	
	Preconditions:
		None
	Postconditions:
		pageGroup index is incremented if the resulting index is valid.
	*/
	virtual void nextPage();

	/*
	BackPage decrements the pageGroup index.
	This method is called whenever buttonBack is used and is intended to be specialized by child objects.

	Preconditions:
		None
	Postconditions:
		pageGroup index is decremented if the resulting index is valid.
	*/
	virtual void backPage();

	/*
	SetIndex changes the closest legal pageGroup index to the targetIndex paraneter.

	Preconditions:
		Accepeted range of targetIndex is (MIN_INT, MAX_INT) which means negative indexes are allowed.
	Postconditions:
		pageGroup index is modified if pageGroup exists.
		pageGroup index is clamped to (0, pageGroup->count() - 1) prior to modification; out-of-bound indexing impossible.
	Returns:
		True if targetIndex was in-range.
		False if targetIndex was out-of-range which also means clamping occurred.
	*/
	bool setIndex(int targetIndex);

	/*
	Returns the layout in this object.
	*/
	virtual QLayout* getLayout();
};

#endif