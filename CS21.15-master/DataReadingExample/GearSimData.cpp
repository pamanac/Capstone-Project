#include "GearSimData.h"

GearSimData::~GearSimData() {
	//Empty intentionally.
}

GearSimData::GearSimData(int index, QString fullName, QString shortName, float value, QString units) {
	this->index = index;
	this->fullName = fullName;
	this->shortName = shortName;
	this->value = value;
	this->units = units;
}

GearSimData::GearSimData(int index, QString combinedName, QString value, QString units) {
	this->index = index;
	QStringList nameList = splitCombinedName(combinedName);
	this->fullName = nameList.front();
	this->shortName = nameList.back();
	this->value = translateScientificNotationIntoFloat(value);
	this->units = units;
}

QStringList GearSimData::splitCombinedName(QString combinedName) {
	QRegularExpression nameSplitter("\\s{2,}|\\t+"); //Used to decipher full names from short names
	QRegularExpressionMatch match = nameSplitter.match(combinedName);

	QStringList fullNameAndShortName = combinedName.split(nameSplitter, Qt::SkipEmptyParts);

	return fullNameAndShortName;
}

float GearSimData::getValue() const {
	return this->value;
}

QString GearSimData::getFullName() const {
	return this->fullName;
}

float GearSimData::translateScientificNotationIntoFloat(QString s) {
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

QString GearSimData::convertToLine() {
	QString returnString = "";
	returnString.append(QString::fromStdString(std::to_string(index)) + QString("\t"));
	returnString.append(fullName + QString("\t"));
	returnString.append(shortName + QString("\t"));
	returnString.append(QString::fromStdString(std::to_string(value)) + QString("\t"));
	returnString.append(units + QString("\t"));
	//qDebug() << returnString;
	return returnString;
}