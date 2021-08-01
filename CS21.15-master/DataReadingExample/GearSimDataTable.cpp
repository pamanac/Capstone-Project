#include "GearSimDataTable.h"

GearSimDataTable::GearSimDataTable(QString tableName, int rowCount, QList<QList<float>> data, QStringList columnNames) {
	this->tableName = tableName;
	this->rowCount = rowCount;
	this->data = data;
	this->columnNames = columnNames;
}

GearSimDataTable::GearSimDataTable(QFile& file) {
	this->rowCount = 0;
	this->buildTableFromFile(file);
}

QList<float> GearSimDataTable::createDataRow(QStringList data) {
	QList<float> newDataPoint;
	for (int i = 0; i < data.size(); i++) {
		newDataPoint.append(translateScientificNotationIntoFloat(data[i]));
	}
	//qDebug() << "Creating data row: " << newDataPoint;
	return newDataPoint;
}

void GearSimDataTable::append(QStringList sList) {
	data.append(createDataRow(sList));
}

bool GearSimDataTable::isValidDataTable() {
	bool isValid = rowCount > 0 && tableName != "" && data.size() == rowCount;
	return isValid;
}

QList<QList<float>> GearSimDataTable::getData() {
	QList<QList<float>> copiedList = data;
	return copiedList;
}

//TODO: functionally decompose this whole function.  It's far too monolithic to be healthy to read.
void GearSimDataTable::buildTableFromFile(QFile &file) {
	QRegularExpression TableStartPattern("[0-9]*");
	QRegularExpression TableEndPattern("END\\stable\\s(.+)");
	QRegularExpression TableDataPattern("-?[0-9]+.?[0-9]*e?-?[0-9]*");
	QRegularExpression TableColumnNamesPattern("[^\\s\\d.]+");
	bool endOfTable = false;
	
	//We use a loop because we don't always know the line of the table's start.
	while (!file.atEnd() && !endOfTable) {
		//Read line
		QByteArray line = file.readLine();

		//Match regex
		QRegularExpressionMatch startMatch = TableStartPattern.match(line);

		//Check for row count and set it.
		if (rowCount == 0 && startMatch.hasMatch()) {
			//Read Row count
			rowCount = startMatch.captured(0).toInt();
			//qDebug() << QString(line);
		} else { //Otherwise, we have data to read.
			//Read column names
			QRegularExpressionMatchIterator columnNamesMatchIterator = TableColumnNamesPattern.globalMatch(line);
			while (columnNamesMatchIterator.hasNext()) {
				QRegularExpressionMatch columnNamesMatch = columnNamesMatchIterator.next();
				//qDebug() << "has a column name match: " << columnNamesMatch.captured(0);
				QString name = columnNamesMatch.captured(0);
				columnNames.append(name);
			}

			//Read numbers
			for (unsigned int i = 0; i < rowCount && !file.atEnd(); i++) {
				//Read row line if not 0
				if (columnNames.size() > 0 || i != 0) {
					line = file.readLine();
				}
				//qDebug() << QString(line);

				//Match with regex for group splitting
				QRegularExpressionMatchIterator dataMatchIterator = TableDataPattern.globalMatch(line);
				QStringList dataLine;
				int numberMatches = 0;
				while(dataMatchIterator.hasNext()) {
					QRegularExpressionMatch dataMatch = dataMatchIterator.next();
					QString probablyANumber = dataMatch.captured(0);
					dataLine << probablyANumber;
					numberMatches++;
				} if (numberMatches > 0) {
					append(dataLine);
				}
			}

			//We must check for row count to verify that we are actively reading data or whitespace.
			if (rowCount > 0) {
				//We can assume the rows have been read, so we must get the table name.
				line = file.readLine();
				QRegularExpressionMatch endMatch = TableEndPattern.match(line);
				tableName = endMatch.captured(1);
				endOfTable = true;

				//qDebug() << QString(line);
			}
		}
	}
}

float GearSimDataTable::translateScientificNotationIntoFloat(QString s) {
	//Check row data for e characters, which indicate scientific notation.
	QRegularExpression scientificNotationSearch("([0-9]+\\.?[0-9]*)e(-?[0-9]+)");
	QRegularExpressionMatch match = scientificNotationSearch.match(s);

	float value = 0.0;
	if (match.hasMatch()) {
		//Since this is the regex for scientific notation, we need to convert into an actual number.
		float decimal = match.captured(1).toFloat();
		float exponent = match.captured(2).toFloat();
		value = pow(decimal, exponent);
	} else {
		value = s.toFloat();
	}

	return value;
}

void GearSimDataTable::fillSeriesWithData(QXYSeries& inputSeries) {
	for (int row = 0; row < data.size(); row++) {
		for (int column = 0; column < data[row].size() - 1; column += 2) {
			qreal x = data[row][column];
			qreal y = data[row][column + 1];
			inputSeries.append(x, y);
		}
	}

	QString seriesName = tableName;
	if (columnNames.size() > 0) {
		seriesName = columnNames[0];
	}
	inputSeries.setName(seriesName);

	return;
}

bool GearSimDataTable::setPoint(int row, int column, float newValue) {
	if (row < data.size() && row > -1 && column < data[row].size() && column > -1) {
		data[row][column] = newValue;
		return true;
	} else {
		return false;
	}
};

QString GearSimDataTable::toString() {
	QString output = tableName + QString("\n");
	
	//Row count
	QString colCount = QString::fromStdString(std::to_string(data.length()));
	output.append(colCount + "\n");

	//Column name writing
	for (int n = 0; n < columnNames.length(); n++) {
		if (columnNames[n] != "") {
			output.append(columnNames[n] + QString("\t"));
		}
	}
	output.append(QString("\n"));
	
	//Data writing
	for (int r = 0; r < data.length(); r++) {
		for (int c = 0; c < data[r].length(); c++) {
			output.append(QString::fromStdString(std::to_string(data[r][c])) + QString("\t"));
		}
		output.append(QString("\n"));
	}
	output.append(QString("END ") + tableName + QString("\n"));
	//qDebug() << output;
	return output;
}

QString GearSimDataTable::getName() {
	return this->tableName;
}

QStringList GearSimDataTable::getColumnNames() {
	return this->columnNames;
}

QChartView* GearSimDataTable::createGraph() {
	QChart* graph = nullptr;
	QChartView* graphView = nullptr;

	if (columnNames.count() == 0) { //If the column names exist, then this is actually a data table.
		graph = new QChart;
		//QSplineSeries* spline = new QSplineSeries;
		QLineSeries* spline = new QLineSeries;

		//Find the range of the x
		float minX = getMinFromColumn(0);
		float maxX = getMaxFromColumn(0);

		//Find the range of the y
		float minY = getMinFromColumn(1);
		float maxY = getMaxFromColumn(1);

		fillSeriesWithData(*spline);
		graph->addSeries(spline);
		graph->createDefaultAxes();
		
		graph->axes(Qt::Horizontal).first()->setRange(minX, maxX);
		graph->axes(Qt::Vertical).first()->setRange(minY, maxY);
		
		QStringList axisName = SplitTableName();
		if (axisName.count() > 1) {
			graph->axes(Qt::Vertical).first()->setTitleText(axisName.front());
			graph->axes(Qt::Horizontal).first()->setTitleText(axisName.back());
		}

		graphView = new QChartView(graph);
	}
	return graphView;
}

//Performs a linear search for maximum
float GearSimDataTable::getMaxFromColumn(int column) {
	float max = FLT_MIN;

	for (int r = 0; r < data.size(); r++) {
		if (data[r].size() < column && column >= 0) {
			//The actual max check
			if (data[r][column] > max) {
				max = data[r][column];
			}
		}
	}

	return max;
}

//Performs a linear search for minimum
float GearSimDataTable::getMinFromColumn(int column) {
	float min = FLT_MAX;

	for (int r = 0; r < data.size(); r++) {
		if (data[r].size() < column && column >= 0) {
			//The actual max check
			if (data[r][column] < min) {
				min = data[r][column];
			}
		}
	}

	return min;
}

QStringList GearSimDataTable::SplitTableName() {
	QRegularExpression nameSplitter("vs\\.?");
	QRegularExpressionMatch match = nameSplitter.match(tableName);

	QStringList axisNames = tableName.split(nameSplitter, Qt::SkipEmptyParts);

	return axisNames;
}