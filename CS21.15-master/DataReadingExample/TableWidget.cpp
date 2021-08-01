#include "TableWidget.h"
#include <QDebug>


tableWidget::tableWidget(QWidget* parent, GearSimDataTable* data): QWidget(parent) {
	//Commented out just to test building
	/*this->model = new tableModel(this);
	this->view = new QTableView(this);

	auto points = data->getData();
	for (QPointF point : points) {
		model->append(point);
	}

	view->setModel(model);
	view->show();*/
}