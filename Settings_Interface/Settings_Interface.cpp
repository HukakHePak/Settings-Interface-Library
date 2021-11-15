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

	virtual ~ISettingsValue() {}

};

class ISettings
{
	map <string, ISettingsValue> list;

	pair <string, ISettingsValue> parseLineInParam(string settingStr)
	{
		



	}

	string writeStringForSave(pair <string, ISettingsValue> item)
	{
		string resultString = item.first + " = ";

		switch (item.second.GetType())
		{	
		case dtUnknown:
			return "";

		case dtBoolean:
			if (item.second.AsBoolean())
				return resultString += "true";
			return resultString += "false";

		case dtString:
			return resultString += "\"" + item.second.AsString() + "\"";
		}
		return item.second.AsString();
	}

public:

	virtual bool LoadFromFile(const string& name)
	{
		ifstream file(name);
		if (!file)
			return false;
		
		while (!file.eof())
		{
			string lineBufStr = "";
			getline(file, lineBufStr);

			auto item = parseLineInParam(lineBufStr);
			list.insert(item);
			
			file.close();
		}
		return true;
	}

	virtual bool SaveToFile(const string& name)
	{
		ofstream file(name);
		if (file)
		{
			for (auto item : list)
				file << writeStringForSave(item) << '\n';

			file.close();
			return true;
		}
		return false;
	}

	virtual ISettingsValue Get(const string& paramName)
	{
		return list[paramName];
	}

	virtual int GetInteger(const string& paramName)
	{
		return list[paramName].AsInteger();
	}

	virtual double GetFloat(const string& paramName)
	{
		return list[paramName].AsDouble();
	}

	virtual bool GetBoolean(const string& paramName) 
	{ 
		list[paramName].AsBoolean();
	}

	virtual string GetString(const string& paramName)
	{
		list[paramName].AsString();
	}

	virtual void SetValue(const string& paramName, const ISettingsValue& value)
	{
		ISettingsValue* newValue = new ISettingsValue;
		newValue->SetValue(dtUnknown, value.AsString);
		list[paramName] = *newValue;
	}

	virtual void SetValue(const string& paramName, int value)
	{
		stringstream valueToString;
		valueToString << value;

		ISettingsValue* newValue = new ISettingsValue;
		newValue->SetValue(dtInteger, valueToString.str());
		list[paramName] = *newValue;
	}

	virtual void SetValue(const string& paramName, double value)
	{
		stringstream valueToString;
		valueToString << value;

		ISettingsValue* newValue = new ISettingsValue;
		newValue->SetValue(dtFloat, valueToString.str());
		list[paramName] = *newValue;
	}

	virtual void SetValue(const string& paramName, bool value)
	{
		stringstream valueToString;
		valueToString << value;

		ISettingsValue* newValue = new ISettingsValue;
		newValue->SetValue(dtBoolean, valueToString.str());
		list[paramName] = *newValue;
	}

	virtual void SetValue(const string& paramName, const char* value)
	{
		string valueToString = value;
		ISettingsValue* newValue = new ISettingsValue;
		newValue->SetValue(dtFloat, valueToString);
		list[paramName] = *newValue;
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
		list[paramName] = *newValue;
	}

	virtual ~ISettings() 
	{
		for (auto item : list)
		{
			delete &item.second;
		}
		list.clear();
	}

};

int main()
{
	DataType b;
	b = dtFloat;
	ISettingsValue a;
	cout << a.GetType() << endl;
	a.SetValue(b, "233");

	cout << a.AsString() << endl;
	cout << a.AsDouble() << endl;
	cout << a.AsInteger() << endl;
    cout << a.AsBoolean() << endl;    
	cout << a.GetType() << endl;
	cout << b << endl;

	system("pause");
}
