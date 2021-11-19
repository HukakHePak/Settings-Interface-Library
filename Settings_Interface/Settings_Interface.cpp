// Settings_Interface.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "ISettings.h"

using namespace std;


namespace ISettings
{
	void ISettingsValue::SetValue(const string& value)
	{
		this->value = value;
		type = dtString;
	}

	void ISettingsValue::SetValue(DataType type, const string& value)
	{
		this->value = value;
		this->type = type;
	}

	string ISettingsValue::AsString() {
		return value;
	}

	int ISettingsValue::AsInteger()
	{
		int result = 0;
		stringstream valueStream(value);
		valueStream >> result;
		return result;
	}

	double ISettingsValue::AsDouble()
	{
		double result = 0;
		stringstream valueStream(value);
		valueStream >> result;
		return result;
	}

	bool ISettingsValue::AsBoolean() {
		return !value.empty();
	}

	DataType ISettingsValue::GetType() {
		return type;
	}

	ISettingsValue::~ISettingsValue()
	{
		value = "";
		value.clear();
	}

	void ISettings::editParamInList(const string& paramName, const string& value, DataType type) {
		list[paramName] = { value, type };
	}

	bool ISettings::isNumber(char symbol) {
		return symbol >= '0' && symbol <= '9';
	}

	bool ISettings::isValIdSymbol(char symbol)
	{
		return symbol == '_'
			|| symbol >= 'a' && symbol <= 'z'
			|| symbol >= 'A' && symbol <= 'Z';
	}

	string ISettings::filterName(const string& name)
	{
		string result = name;

		for (unsigned int i = 0; i < result.length(); i++)
		{
			if (isValIdSymbol(result[i]))
				break;
			result[i] = ' ';
		}

		int lastPos = 0;
		for (auto i : result)
			if (isValIdSymbol(i) || isNumber(i))
				result[lastPos++] = i;

		return result.erase(lastPos, result.length());
	}

	string ISettings::paramToStrForSave(const parameter& param)
	{
		switch (param.type)
		{
		case dtUnknown:
			return "";

		case dtBoolean:
			return param.value.empty() ? "false" : "true";

		case dtString:
			return "\"" + param.value + "\"";
		}
		return param.value;
	}

	DataType ISettings::checkStrType(const string& str)
	{
		if (str[0] == '\"')
			return dtString;

		if (isNumber(str[0]) || str[0] == '-')
		{
			for (auto i : str)
				if (i == '.')
					return dtFloat;
			return dtInteger;
		}

		if (str == "true" || str == "false")
			return dtBoolean;

		return dtUnknown;
	}

	string ISettings::parseStrByType(const string& str, DataType type)
	{
		switch (type)
		{
		case dtInteger:
		case dtFloat:
			return str;

		case dtBoolean:
			return str.compare("true") ? "" : "0";

		case dtString:
			int end = str.find('\"', 1);
			return str.substr(1, end - 1);
		}
		return "";
	}



	bool ISettings::LoadFromFile(const string& name)
	{
		ifstream file(name);
		if (!file)
			return false;

		while (!file.eof())
		{
			string paramName = "";
			file >> paramName;

			string fileLine = "";
			getline(file, fileLine);

			if (fileLine.empty())
				continue;

			fileLine = fileLine.substr(3);		// cut " = "

			DataType paramType = checkStrType(fileLine);
			string paramValue = parseStrByType(fileLine, paramType);
			editParamInList(paramName, paramValue, paramType);
		}
		file.close();
		return true;
	}

	bool ISettings::SaveToFile(const string& name)
	{
		if (list.empty())
			return true;

		ofstream file(name);
		if (file)
		{
			for (auto item : list)
				file << filterName(item.first) << " = " << paramToStrForSave(item.second) << '\n';

			file.close();
			return true;
		}
		return false;
	}

	ISettingsValue* ISettings::Get(const string& paramName)
	{
		ISettingsValue* value;
		parameter* param = &list[paramName];
		value->SetValue(param->type, param->value);
		return value;
	}

	int ISettings::GetInteger(const string& paramName)
	{
		int value = 0;
		stringstream stream(list[paramName].value);
		stream >> value;
		return value;
	}

	double ISettings::GetFloat(const string& paramName)
	{
		double value = 0;
		stringstream stream(list[paramName].value);
		stream >> value;
		return value;
	}

	bool ISettings::GetBoolean(const string& paramName) {
		return !list[paramName].value.empty();
	}

	string ISettings::GetString(const string& paramName) {
		return list[paramName].value;
	}

	void ISettings::SetValue(const string& paramName, ISettingsValue& value) {
		editParamInList(paramName, value.AsString(), value.GetType());
	}

	void ISettings::SetValue(const string& paramName, int value)
	{
		stringstream valueToString;
		valueToString << value;
		editParamInList(paramName, valueToString.str(), dtInteger);
	}

	void ISettings::SetValue(const string& paramName, double value)
	{
		stringstream valueToString;
		valueToString << value;
		editParamInList(paramName, valueToString.str(), dtFloat);
	}

	void ISettings::SetValue(const string& paramName, bool value)
	{
		editParamInList(paramName, value ? "0" : "", dtBoolean);
	}

	void ISettings::SetString(const string& paramName, const string& value) {
		editParamInList(paramName, value, dtString);
	}

	void ISettings::SetValue(const string& paramName, const char* value) {
		SetString(paramName, value);
	}

	void ISettings::SetInteger(const string& paramName, int value) {
		SetValue(paramName, value);
	}

	void ISettings::SetFloat(const string& paramName, double value) {
		SetValue(paramName, value);
	}

	void ISettings::SetBoolean(const string& paramName, bool value) {
		SetValue(paramName, value);
	}

	ISettings::~ISettings() { list.clear(); }
}