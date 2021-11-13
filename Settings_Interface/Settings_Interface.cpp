// Settings_Interface.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <fstream>
#include <map>
#include <string>

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
	string value;
	DataType type;

public:
	ISettingsValue(const string& value = "") { SetValue(value); }

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
		try { return stoi(value); }
		catch (std::invalid_argument e)	{ return 0; }
	}

	virtual double AsDouble()
	{
		try { return stod(value); }
		catch (std::invalid_argument e)	{ return 0; }		
	}

	virtual bool AsBoolean() { return !value.empty(); }

	virtual DataType GetType() { return type; }

	virtual ~ISettingsValue() {}

};

class ISettings
{
	string value;

	struct parameter {
		string value;
		DataType type;
	};
	
	map <string, parameter> list;

public:

	virtual bool LoadFromFile(const string& name)
	{

	}

	virtual bool SaveToFile(const string& name) = 0;

	virtual ISettingsValue Get(const string& paramName) = 0;

	virtual int GetInteger(const string& paramName)
	{
		try { return stoi(value); }
		catch (std::invalid_argument e) { return 0; }
	}

	virtual double GetFloat(const string& paramName)
	{
		try { return stod(value); }
		catch (std::invalid_argument e) { return 0; }
	}

	virtual bool GetBoolean(const string& paramName) 
	{ 
		return !value.empty(); 
	}

	virtual string GetString(const string& paramName)
	{
		return value;
	}

	virtual void SetValue(const string& paramName, const ISettingsValue& value) = 0;

	virtual void SetValue(const string& paramName, int value) = 0;

	virtual void SetValue(const string& paramName, double value) = 0;

	virtual void SetValue(const string& paramName, bool value) = 0;

	virtual void SetValue(const string& paramName, const char* value) = 0;

	virtual void SetInteger(const string& paramName, int value) = 0;

	virtual void SetFloat(const string& paramName, double value) = 0;

	virtual void SetBoolean(const string& paramName, bool value) = 0;

	virtual void SetString(const string& paramName, const string& value) = 0;

	virtual ~ISettings() {};

};

int main()
{
	ISettingsValue a;
	a.SetValue("");

	cout << a.AsString() << endl;
    cout << a.AsBoolean() << endl;    
	cout << a.AsDouble () << endl;
	cout << a.AsInteger() << endl;
	system("pause");
}
