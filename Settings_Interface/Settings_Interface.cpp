// Settings_Interface.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <sstream>

using namespace std;

enum DataType
{
	dtInteger,
	dtFloat,
	dtBoolean,
	dtString,
	dtUnknown
};

class ISettingsValue
{
	string value = "";
	DataType type = dtUnknown;

public:
	void SetValue(const string& value)
	{
		this->value = value;
		type = dtString;
	}

	virtual void SetValue(DataType type, const string& value)
	{
		this->value = value;
		this->type = type;
	}

	virtual string AsString() { 
		return value; 
	}

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

	virtual bool AsBoolean() { 
		return !value.empty(); 
	}

	virtual DataType GetType() { 
		return type;
	}

	virtual ~ISettingsValue() 
	{
		value = "";
		value.clear();
	}
};

class ISettings
{
	struct parameter 
	{
		string value = "";
		DataType type = dtUnknown;
	};

	map <string, parameter> list;

	void editParamInList(const string& paramName, const string& value, DataType type) {
		list[paramName] = { value, type };
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

	string filterString(const string& name)
	{
		string result = name;

		if (isNumber(result[0]))
			result[0] = ' ';

		int lastPos = 0;
		for (auto i : result)
			if (isValIdSymbol(i) || isNumber(i))
			{
				result[lastPos] = i;
				lastPos++;
			}

		return result.erase(lastPos, result.length());
	}

	string paramToStrForSave(const parameter& param)
	{
		switch (param.type)
		{	
		case dtUnknown:
			return "";

		case dtBoolean:
			return param.value.empty() ?  "false" : "true";

		case dtString:
			return "\"" + param.value + "\"";
		}
		return param.value;
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

		if (str == "true" || str == "false")
			return dtBoolean;

		return dtUnknown;
	}

	string parseStrByType(const string& str, DataType type)
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

public:

	virtual bool LoadFromFile(const string& name)
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

	virtual bool SaveToFile(const string& name)
	{
		if (list.empty())
			return true;

		ofstream file(name);
		if (file)
		{
			for (auto item : list)
			{
				string paramName = filterString(item.first);
				string paramValue = paramToStrForSave(item.second);

				if(paramValue != "")
					file << paramName << " = " << paramValue << '\n';
			}
			file.close();
			return true;
		}
		return false;
	}

	virtual ISettingsValue* Get(const string& paramName)
	{
		ISettingsValue* value = new ISettingsValue;
		parameter* param = &list[paramName];
		value->SetValue(param->type, param->value);
		return value;
	}

	virtual int GetInteger(const string& paramName)
	{
		int value = 0;
		stringstream stream(list[paramName].value);
		stream >> value;
		return value;
	}

	virtual double GetFloat(const string& paramName)
	{
		double value = 0;
		stringstream stream(list[paramName].value);
		stream >> value;
		return value;
	}

	virtual bool GetBoolean(const string& paramName) { 
		return !list[paramName].value.empty();
	}

	virtual string GetString(const string& paramName) {
		return list[paramName].value;
	}

	virtual void SetValue(const string& paramName, ISettingsValue& value) {
		editParamInList(paramName, value.AsString(), value.GetType());
	}

	virtual void SetValue(const string& paramName, int value)
	{
		stringstream valueToString;
		valueToString << value;
		editParamInList(paramName, valueToString.str(), dtInteger);
	}

	virtual void SetValue(const string& paramName, double value)
	{
		stringstream valueToString;
		valueToString << value;
		editParamInList(paramName, valueToString.str(), dtFloat);
	}

	virtual void SetValue(const string& paramName, bool value)
	{
		stringstream valueToString;
		valueToString << value;
		editParamInList(paramName, valueToString.str(), dtBoolean);
	}


	virtual void SetString(const string& paramName, const string& value) {
		editParamInList(paramName, value, dtString);
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

	virtual ~ISettings() { list.clear(); }
};

int main()
{
	ISettingsValue a;
	a.SetValue("true");

	ISettings set;
	/*set.SetValue("1first parameter", a);
	set.SetValue("second_pa 24rametE]\nr", 3.2);
	set.SetValue("third_parameter", "my o\nwn");
	set.SetValue("fifth_parameter", "");
	set.SetValue("second_parameter", 2);
	set.SetValue("fourth_parameter", true);*/

	set.LoadFromFile("test.txt");
	set.SaveToFile("test.txt");
}
