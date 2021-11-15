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

	virtual string AsString() { return value; }

	virtual int AsInteger()
	{
		stringstream valueStream(value);
		int result = 0;
		valueStream >> result;
		return result;
	}

	virtual double AsDouble()
	{	
		stringstream valueStream(value);
		double result = 0;
		valueStream >> result;
		return result;
	}

	virtual bool AsBoolean()
	{
		return !value.empty();
	}

	virtual DataType GetType() { return type; }

	virtual ~ISettingsValue() 
	{
		value = "";
		value.clear();
	}
};

class ISettings
{
	map <string, ISettingsValue*> list;

	bool isNumber(char symbol)
	{
		return symbol >= '0' && symbol <= '9' || symbol == '=';
	}

	ISettingsValue* strToISettingsValueParser(string settingStr)
	{
		stringstream settingStrStream(settingStr);
		string strBuf = settingStr;
		string paramValueStr = "";
		DataType paramType = dtUnknown;

		if (strBuf[0] == '\"')
		{
			strBuf = strBuf.substr(1);
			for (auto i : strBuf)
			{
				if (i == '\"')
					break;
				paramValueStr += i;
			}
			if (paramValueStr != "")
				paramType = dtString;					   
		}
		else
		{
			settingStrStream >> paramValueStr;
			if (isNumber(paramValueStr[0]))
			{
				paramType = dtInteger;
				for (auto i : paramValueStr)
				{
					if (i == '.')
					{
						paramType = dtFloat;
						break;
					}
				}
			}
			else if (paramValueStr == "true" || paramValueStr == "false")
				paramType = dtBoolean;
		}
		ISettingsValue* paramValue = new ISettingsValue;
		paramValue->SetValue(paramType, paramValueStr);
		return paramValue;
	}

	char isValIdSymbol(char symbol)
	{
		return symbol >= 'a' && symbol <= 'z' 
			|| symbol >= 'A' && symbol <= 'Z' 
			|| symbol == '_';
	}

	string nameValidator(string name)
	{
		string validName = "";
		for (auto i : name)
		{
			if (isValIdSymbol(i) || isNumber(i))
				validName += i;
		}
		if (isNumber(validName[0]))
			validName = validName.substr(1);
		return validName;
	}

	string writeStringForSave(pair <string, ISettingsValue*> item)
	{
		string validName = nameValidator(item.first);
		string resultString = validName + " = ";

		switch (item.second->GetType())
		{	
		case dtUnknown:
			return "";

		case dtBoolean:
			if (item.second->AsBoolean())
				return resultString += "true";
			return resultString += "false";

		case dtString:
			return resultString += "\"" + item.second->AsString() + "\"";
		}
		return resultString += item.second->AsString();
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

			string lineBufStr = "";
			getline(file, lineBufStr);
			if (lineBufStr.empty())
				continue;

			lineBufStr = lineBufStr.substr(3);		// cut " = "

			ISettingsValue* paramValue = strToISettingsValueParser(lineBufStr);
			if (paramValue->GetType() == dtUnknown)
			{
				delete paramValue;
				continue;
			}			
			list[paramName] = paramValue;
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
				if(item.second->AsString() != "")
					file << writeStringForSave(item) << '\n';

			file.close();
			return true;
		}
		return false;
	}

	virtual ISettingsValue Get(const string& paramName)
	{
		return *list[paramName];
	}

	virtual int GetInteger(const string& paramName)
	{
		return list[paramName]->AsInteger();
	}

	virtual double GetFloat(const string& paramName)
	{
		return list[paramName]->AsDouble();
	}

	virtual bool GetBoolean(const string& paramName) 
	{ 
		return list[paramName]->AsBoolean();
	}

	virtual string GetString(const string& paramName)
	{
		return list[paramName]->AsString();
	}

	virtual void SetValue(const string& paramName, const ISettingsValue& value)
	{
		ISettingsValue* newValue = new ISettingsValue;
		*newValue = value;
		list[paramName] = newValue;
	}

	virtual void SetValue(const string& paramName, int value)
	{
		stringstream valueToString;
		valueToString << value;

		ISettingsValue* newValue = new ISettingsValue;
		newValue->SetValue(dtInteger, valueToString.str());
		list[paramName] = newValue;
	}

	virtual void SetValue(const string& paramName, double value)
	{
		stringstream valueToString;
		valueToString << value;

		ISettingsValue* newValue = new ISettingsValue;
		newValue->SetValue(dtFloat, valueToString.str());
		list[paramName] = newValue;
	}

	virtual void SetValue(const string& paramName, bool value)
	{
		stringstream valueToString;
		valueToString << value;

		ISettingsValue* newValue = new ISettingsValue;
		newValue->SetValue(dtBoolean, valueToString.str());
		list[paramName] = newValue;
	}

	virtual void SetValue(const string& paramName, const char* value)
	{
		string valueToString = value;
		ISettingsValue* newValue = new ISettingsValue;
		newValue->SetValue(dtString, valueToString);
		list[paramName] = newValue;
	}

	virtual void SetInteger(const string& paramName, int value)
	{
		SetValue(paramName, value);
	}

	virtual void SetFloat(const string& paramName, double value)
	{
		SetValue(paramName, value);
	}

	virtual void SetBoolean(const string& paramName, bool value)
	{
		SetValue(paramName, value);
	}

	virtual void SetString(const string& paramName, const string& value)
	{
		ISettingsValue* newValue = new ISettingsValue;
		newValue->SetValue(value);
		list[paramName] = newValue;
	}

	virtual ~ISettings() 
	{
		for (auto item : list)
		{
			delete item.second;
		}
		list.clear();
	}
};

int main()
{
	ISettingsValue a;
	a.SetValue("true");

	ISettings set;
	/*set.SetValue("1first parameter", a);
	set.SetValue("second_pa 24rametE]\nr", 3.2);
	set.SetValue("third_parameter", "my own");
	set.SetValue("fifth_parameter", "");

	set.SetValue("second_parameter", 2);
	cout << a.AsString();

	set.SaveToFile("test.txt");
*/
	set.LoadFromFile("test.txt");
	set.SaveToFile("test.txt");
}
