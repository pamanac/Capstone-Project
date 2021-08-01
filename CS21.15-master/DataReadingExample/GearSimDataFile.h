#pragma once
#include<QtCore>
#include<QtCharts>
#include"GearSimData.h"
#include"GearSimDataTable.h"

/*
* Class to contain a gearSim file's data.
*/
class GearSimDataFile {
public:
	GearSimDataFile(QString fileName);
	~GearSimDataFile();

	/*
	* Accessor for Variables list
	*/
	QList<GearSimData*> getVariablesList();

	/*
	* Accessor for data tables list
	*/
	QList<GearSimDataTable*> getDataTablesList();

	/*
	* Accessor for file name
	*/
	QString getFileName();

	/*
	* Writes to disk.
	* False for failure, true for success.
	*/
	bool writeToDisk();

	bool isValidFile();
private:
	/*
	* QFile creation function which merely creates a QFile from a given filename.
	* Returns nullptr if file fails to open.
	* TODO: allow options for writing to files. Currently locks to readOnly.
	*/
	QFile* createFile(QString fileName, bool fileNameIsAbsolutePath);

	/*
	* Helper function to determine if the end of the variable section in a gearsim file was reached.
	*/
	bool isEndOfVariableData(QString fileLine);

	/*
	* Creates GearSim data from input lines. Said lines expect GearSim .DAT format.
	* Returns nullptr if data line doesn't match known formats.
	*/
	GearSimData* gearSimDataFromFileLine(QString fLine);

	/*
	* Reads data from given file object.
	*/
	QList<GearSimData*> readGearSimData(QFile& file, bool closeFileWhenDone);

	/*
	* Reads dataTables from given file object.
	*/
	QList<GearSimDataTable*> gearSimTablesFromFile(QFile& file);

	QList<GearSimData*> Variables;
	QList<GearSimDataTable*> DataTables;
	QString fileName;
	QString fileHeaderLine; //Some line at the start of the file.

	//TODO: Method for writing
	//TODO: Method for reading
};