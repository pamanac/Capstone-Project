#pragma once
#include<QtCore>

/*
* Data class used to hold info about data lines read from GearSIm data files.
*/
class GearSimData {
public:
	/*
	* The second constructor in this object is the primary intended use case.
	* It will automatically translate variables into the right data types.
	*/
	GearSimData(int index, QString fullName, QString shortName, float value, QString units);
	GearSimData(int index, QString combinedName, QString value, QString units);
	~GearSimData();

	QString convertToLine();

//private:
	int index; //GearSim has an order to variables, so we store it as an index.
	QString fullName;
	QString shortName;
	float value;
	QString units;

	float getValue() const;
	QString getFullName() const;

private:
	/*
	* Helper function for splitting full name from short name.
	*/
	QStringList splitCombinedName(QString combinedName);

	/*
	* Helper function to convert scientific notation into floats. 
	* GearSim scientific notation follows the form <decimal>e<exponent>. This function expects that as a string.
	* If string fails to match, it provides 0.0 as an output.
	*/
	float translateScientificNotationIntoFloat(QString s);
};