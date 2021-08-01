#include"ExampleDataLineWidget.h"

labeledLineEditWidget::labeledLineEditWidget(QWidget* parent, QString dataLineStartLabelText, QString dataLineText, QString dataLineEndLabelText, QString dataLineInputMask)
: QWidget(parent) {
	//Setup the start label text
	dataLineStartLabel = new QLabel;
	dataLineStartLabel->setText(dataLineStartLabelText);
	
	//Setup the placeholder text
	dataLine = new QLineEdit;
	dataLine->setPlaceholderText(dataLineText);
	dataLine->setInputMask(dataLineInputMask);

	//Setup the end label text
	dataLineEndLabel = new QLabel;
	dataLineEndLabel->setText(dataLineEndLabelText);


	//Setup the layout
	dataLineLayout = new QHBoxLayout;
	dataLineLayout->addWidget(dataLineStartLabel);
	dataLineLayout->addWidget(dataLine);
	dataLineLayout->addWidget(dataLineEndLabel);
	this->setLayout(dataLineLayout);
}