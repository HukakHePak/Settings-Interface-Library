#include "ISettings.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <map>

class SettingsValue : public ISettingsValue
{
	string value = "";
	DataType type = dtUnknown;

public:
	SettingsValue() {}
	SettingsValue(const string& value) : value(value) {}

	virtual void SetValue(const string& value)
	{
		this->value = value;
		type = dtString;
	}

	virtual void SetValue(DataType type, const string& value)
	{
		this->value = value;
		this->type = type;
	}

	virtual string AsString() {	return value; }

	virtual int AsInteger()
	{
		int result = 0;
		stringstream valueStream(value);
		valueStream >> result;
		return result;
	}

	virtual double AsDouble()
	{
		double result = 0;
		stringstream valueStream(value);
		valueStream >> result;
		return result;
	}

	virtual bool AsBoolean() { return !value.empty(); }

	virtual DataType GetType() { return type; }

	virtual ~SettingsValue()
	{
		value = "";
		value.clear();
		cout << "delete_value" << endl;
	}
};

class Settings : public ISettings
{
	map <string, SettingsValue> list;

	void editParamInList(const string& paramName, const string& value, DataType type) {
		list[paramName].SetValue(type, value);
	}

	bool isNumber(char symbol) {
		return symbol >= '0' && symbol <= '9';
	}

	bool isValIdSymbol(char symbol)
	{
		return symbol == '_'
			|| symbol >= 'a' && symbol <= 'z'
			|| symbol >= 'A' && symbol <= 'Z';
	}

	string filter(const string& str, bool (*checkSymbol) (char))
	{
		string result = str;
		int lastPos = 0;

		for (auto i : result)
			if (checkSymbol(i))
				result[lastPos++] = i;

		return result.erase(lastPos);
	}

	string filterName(const string& name)
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

		return result.erase(lastPos);
	}

	string valueToStrForSave(string value, DataType type)
	{
		switch (type)
		{
		case dtUnknown:
			return "";

		case dtBoolean:
			return value.empty() ? "false" : "true";

		case dtString:
			return "\"" + value + "\"";
		}
		return value;
	}

	DataType checkStrType(const string& str)
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

		if (str.find("true") != -1 || str.find("false") != -1)
			return dtBoolean;

		return dtUnknown;
	}

	string parseStrByType(const string& str, DataType type)
	{
		switch (type)
		{
		case dtInteger:
		case dtFloat:
			return filter(str, [](char c) -> bool { return c >= '0' && c <= '9' || c == '.'; });

		case dtBoolean:
			return str.find("true") == -1 ? "" : "0";

		case dtString:
			int end = str.find('\"', 1);
			return str.substr(1, end - 1);
		}
		return "";
	}

public:

	virtual bool LoadFromFile(const string& name)
	{
		ifstream file(name);
		if (!file)
			return false;

		while (!file.eof())
		{
			string line = "";
			getline(file, line);

			if (line.empty())
				continue;

			int lineDiv = line.find('=');
			string paramName = filterName(line.substr(0, lineDiv));
			string lineValue = line.substr(lineDiv + 1);

			int startPos = 0;
			while (lineValue[startPos] == ' ')
				startPos++;
			lineValue = lineValue.substr(startPos);

			DataType paramType = checkStrType(lineValue);
			string paramValue = parseStrByType(lineValue, paramType);
			list[paramName].SetValue(paramType, paramValue);
		}
		file.close();
		return true;
	}

	virtual bool SaveToFile(const string& name)
	{
		if (list.empty())
			return true;

		ofstream file(name);
		if (file)
		{
			for (auto item : list)
				file << item.first << " = " 
				<< valueToStrForSave(item.second.AsString(), item.second.GetType()) << '\n';

			file.close();
			return true;
		}
		return false;
	}

	virtual ISettingsValue& Get(const string& paramName) { return list[paramName]; }

	virtual int GetInteger(const string& paramName)	{ return list[paramName].AsInteger(); }

	virtual double GetFloat(const string& paramName) { return list[paramName].AsDouble(); }

	virtual bool GetBoolean(const string& paramName) { return !list[paramName].AsBoolean(); }

	virtual string GetString(const string& paramName) {	return list[paramName].AsString(); }

	virtual void SetValue(const string& paramName, ISettingsValue& value) 
	{
		list[filterName(paramName)].SetValue(value.GetType(), value.AsString()); 
	}

	virtual void SetValue(const string& paramName, int value)
	{
		stringstream valueToString;
		valueToString << value;
		list[filterName(paramName)].SetValue(dtInteger, valueToString.str());
	}

	virtual void SetValue(const string& paramName, double value)
	{
		stringstream valueToString;
		valueToString << value;
		list[filterName(paramName)].SetValue(dtFloat, valueToString.str());
	}

	virtual void SetValue(const string& paramName, bool value)
	{
		list[filterName(paramName)].SetValue(dtBoolean, value ? "0" : "");
	}

	virtual void SetString(const string& paramName, const string& value) {
		list[filterName(paramName)].SetValue(value);
	}

	virtual void SetValue(const string& paramName, const char* value) {
		SetString(paramName, value);
	}

	virtual void SetInteger(const string& paramName, int value) {
		SetValue(paramName, value);
	}

	virtual void SetFloat(const string& paramName, double value) {
		SetValue(paramName, value);
	}

	virtual void SetBoolean(const string& paramName, bool value) {
		SetValue(paramName, value);
	}

	virtual ~Settings() { list.clear(); }
};