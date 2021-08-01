#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QMenuBar>
#include <qstackedwidget.h>
#include <QDockWidget>
class MainWindow : public QMainWindow { //TODO: make a more clear name for this class.
	Q_OBJECT
public:
	MainWindow(QWidget* parent);
	QMenuBar* generateMenu();

	QStackedWidget* getStack();
	QDockWidget* getDock();

	~MainWindow();

	struct ProgramState {
		struct UserSelections {
			QString airplane;
			QString simulation;
		};
		UserSelections userSelections;
	};
	ProgramState programState;

private:
	QStackedWidget* stack; //always centralWidget()
	QDockWidget* dock;
};

#endif