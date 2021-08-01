#include"GearSimDataFile.h"
GearSimDataFile::~GearSimDataFile() {
	//Intentionally Empty
}

GearSimDataFile::GearSimDataFile(QString fileName) {
	this->fileName = fileName;
	fileHeaderLine = "";

	//Open the file
	QFile* GearSimFile = createFile(fileName, true);

	//Validate file opened
	if (GearSimFile) {
		//Generate GearSimDatas
		this->Variables = readGearSimData(*GearSimFile, false);

		//Generate GearSimDataTables
		this->DataTables = gearSimTablesFromFile(*GearSimFile);
	}
}

QList<GearSimData*> GearSimDataFile::getVariablesList() {
	QList<GearSimData*> copiedList = Variables;
	return copiedList;
}

QFile* GearSimDataFile::createFile(QString fileName, bool fileNameIsAbsolutePath) {

	//Convert fileName to a full path if necessary
	if (!fileNameIsAbsolutePath) {
		fileName = QDir::currentPath().append('/').append(fileName);
	}

	//Open file
	QFile* file = new QFile(fileName);
	qDebug() << "Reading file " << fileName;
	if (!file->open(QIODevice::ReadOnly | QIODevice::Text)) {
		delete file;
		return nullptr;
	}

	//Get the first line of the file
	fileHeaderLine = file->readLine();

	return file;
}

QList<GearSimDataTable*> GearSimDataFile::getDataTablesList() {
	QList<GearSimDataTable*> copiedList = DataTables;
	return copiedList;
}

/*
* Helper function to determine if the end of the variable section in a gearsim file was reached.
*/
bool GearSimDataFile::isEndOfVariableData(QString fileLine) {
	QRegularExpression sectionEndPattern("End\\s*auto-generated\\s*variable\\s*section");
	QRegularExpressionMatch match = sectionEndPattern.match(fileLine);

	bool result = false;

	if (match.hasMatch()) {
		result = true;
	}

	return result;
}

QString GearSimDataFile::getFileName() {
	return this->fileName;
}

QList<GearSimDataTable*> GearSimDataFile::gearSimTablesFromFile(QFile& file) {
	QList<GearSimDataTable*> dataTables;
	GearSimDataTable* currentTable = nullptr;

	while (!file.atEnd()) {
		//ask current table to read
		currentTable = new GearSimDataTable(file);
		if (currentTable->isValidDataTable()) {
			dataTables.append(currentTable);
			qDebug() << "Constructed valid table called " << currentTable->getName() << " with column names " << currentTable->getColumnNames();
		} else {
			qDebug() << "Constructed invalid table called " << currentTable->getName() << " with column names " << currentTable->getColumnNames();
		}
	}

	return dataTables;
}

/*
* Creates GearSim data from input lines. Said lines expect GearSim .DAT format.
* Returns nullptr if data line doesn't match known formats.
*/
GearSimData* GearSimDataFile::gearSimDataFromFileLine(QString fLine) {
	GearSimData* dataLine = nullptr;

	//C++ string rules require two backslashes to represent the character '\'
	QRegularExpression normPattern("([0-9]+)\\s+(.*)\\s+([0-9]\\.[0-9]+|-[0-9]+|[0-9]+)\\s+(.*)");
	QRegularExpression sciePattern("([0-9]+)\\s+(.*)\\s+([0-9]+\\.?[0-9]*)e(-?[0-9]+)\\s+(.*)");

	QRegularExpressionMatch match = normPattern.match(fLine);
	if (match.hasMatch()) { //Convert variable entries.
		//<index of value> <Full name of value> <Shortname of value> <actual value> <units>
		int index = match.captured(1).toInt();
		QString fullNameAndShortName = match.captured(2);
		QString value = match.captured(3);
		QString units = match.captured(4);
		dataLine = new GearSimData(index, fullNameAndShortName, value, units);
		qDebug() << dataLine->index << dataLine->fullName << dataLine->shortName << dataLine->value << dataLine->units;

	} else { //If there was no match with the normal pattern, there might be a scientific notation match.
		match = sciePattern.match(fLine);
		if (match.hasMatch()) {
			int index = match.captured(1).toInt();
			QString fullNameAndShortName = match.captured(2);
			QString units = match.captured(5);
			QString decimal = match.captured(3);
			QString exponent = match.captured(4);
			QString value = decimal + "e" + exponent;

			dataLine = new GearSimData(index, fullNameAndShortName, value, units);
			qDebug() << dataLine->index << dataLine->fullName << dataLine->shortName << dataLine->value << dataLine->units;
		}
	}

	return dataLine;
}

/*
* Reads data from given file object.
*/
QList<GearSimData*> GearSimDataFile::readGearSimData(QFile& file, bool closeFileWhenDone) {
	QList<GearSimData*> dataFileContents;
	bool variableDataEndReached = false;

	//Fill the list from the file
	while (!file.atEnd() && !variableDataEndReached) {
		QByteArray line = file.readLine();
		GearSimData* translatedLine = gearSimDataFromFileLine(line);

		if (translatedLine != nullptr) {
			dataFileContents.append(translatedLine);
		} else {
			//Check if this is end of the variables.
			if (isEndOfVariableData(line)) {
				variableDataEndReached = true;
			}
		}
	}

	//Close file if requested
	if (closeFileWhenDone) {
		//Close the file and return the widgets
		qDebug() << "Closing file " << file.fileName();
		file.close();
	}

	return dataFileContents;
}

bool GearSimDataFile::writeToDisk() {
	//Open some file
	qDebug() << "Writing to file...";
	QFile* file = new QFile(fileName);
	if(!file->open(QIODevice::WriteOnly | QIODevice::Text)){
		return false;
	}

	QString firstLine = fileHeaderLine + "\n\n";
	file->write(firstLine.toLatin1());

	//Write variables
	file->write("Automatically generated input file...\n");
	for( GearSimData* datum : this->Variables){
		QString output = datum->convertToLine() + QString("\n");
		file->write(output.toLatin1());
	}
	file->write("\n\n\nEnd auto-generated variable section...\n\n");

	//Write tables
	int tableIndex = 1;
	for( GearSimDataTable* table : this->DataTables){
		QString output = table->toString() + QString("\n\n");
		file->write(output.toLatin1());
		//file->write("End of table " + tableIndex + '\n');
		tableIndex++;
	}

	file->close();
	return true;
}

bool GearSimDataFile::isValidFile() {
	QFile* fptr = createFile(fileName, false);
	if (fptr) {
		fptr->close();
		return true;
	}
	return false;
}