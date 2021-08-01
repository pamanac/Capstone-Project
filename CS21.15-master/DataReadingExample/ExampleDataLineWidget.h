#pragma once
#include <QtWidgets>
#include <QtCore>

class labeledLineEditWidget : public QWidget {
	Q_OBJECT
public:
	labeledLineEditWidget(QWidget* parent = 0, QString dataLineStartLabelText = "Placeholder:", QString dataLineText = "Hello", QString dataLineEndLabelText = "World", QString dataLineInputMask = "");
	
	QLineEdit* dataLine;
	QLabel* dataLineStartLabel;
	QLabel* dataLineEndLabel;
	QHBoxLayout* dataLineLayout;
};