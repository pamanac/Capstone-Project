#pragma once
#include<QtCore>
#include<QtCharts>

/*
* Data class used to hold info about data lines read from GearSIm data files.
*/
class GearSimDataTable {
public:
	GearSimDataTable(QString tableName, int rowCount, QList<QList<float>> data, QStringList columnNames);
	GearSimDataTable(QFile& file);

	/*
	* Uses the createDataRow function and then appends the result to the internal data list.
	*/
	void append(QStringList sList);

	/*
	* Allows checking if the table has all expected values filled.  If it doesn't, then data was given incorrectly.
	*/
	bool isValidDataTable();

	/*
	* Accessors for data
	*/
	QList<QList<float>> getData();
	QString getName();
	QStringList getColumnNames();

	/*
	* Function to generate a graph.
	* Nullptr will be returned if the graph creation fails.
	*/
	QChartView* createGraph();

	/*
	* Intended to help find the limits on graph ranges
	* Returns MIN_FLT if failed.
	*/
	float getMaxFromColumn(int column);
	
	/*
	* Intended to help find the limits on graph ranges
	* Returns MAX_FLT if failed.
	*/
	float getMinFromColumn(int column);

	/*
	* Helper function that converts the table data into point pairs via a given QXYSeries.
	* Any child of a QXYSeries that supports append(float x, float Y) and setName(QString name) will work.
	* 
	* Naturally, this function will change the input object.
	*/
	void fillSeriesWithData(QXYSeries& inputSeries);

	/*
	* Setter for positions in table.
	* Returns true if valid row-column combination.  False otherwise.
	*/
	bool setPoint(int row, int column, float newValue);

	QString toString();

private:
	QString tableName;
	int rowCount;
	QList<QList<float>> data;
	QStringList columnNames;

	/*
	* Makes a QList<float> from a string list whose entries are the form <decimal>e<exponent> or <decimal>
	* Said QList is then appended to the data table.
	*/
	QList<float> createDataRow(QStringList data);

	/*
	* Reads data table from QFile object. Expects row count to be 0.
	* 
	* SDI data table format:
	*	<Table name>
	*	<number of rows>
	*	<title of each column (only present if it was a data table and not a graph)>
	*	<X> <y> <z>
	*	<X> <y> <z>
	*	...
	*	END table <Table name>
	*/
	void buildTableFromFile(QFile &file);
	
	/* Note: identical to function from GearSimData object.
	* Helper function to convert scientific notation into floats.
	* GearSim scientific notation follows the form <decimal>e<exponent>. This function expects that as a string.
	* If string fails to match, it provides 0.0 as an output.
	*/
	float translateScientificNotationIntoFloat(QString s);

	/*
	* Used to help build graph labels.  Assumes table has name with "vs." in it.
	*/
	QStringList SplitTableName();
};