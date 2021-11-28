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
	SettingsValue();
	SettingsValue(const string& value);

	virtual void SetValue(const string& value);

	virtual void SetValue(DataType type, const string& value);

	virtual string AsString();

	virtual int AsInteger();

	virtual double AsDouble();

	virtual bool AsBoolean();

	virtual DataType GetType();

	virtual ~SettingsValue();
};

class Settings : public ISettings
{
	map <string, SettingsValue> list;

protected:

	void editParamInList(const string& paramName, const string& value, DataType type);

	bool isNumber(char symbol);

	bool isValIdSymbol(char symbol);

	string filter(const string& str, bool(*checkSymbol) (char));

	string filterName(const string& name);

	string valueToStrForSave(string value, DataType type);

	DataType checkStrType(const string& str);

	string parseStrByType(const string& str, DataType type);

public:

	virtual bool LoadFromFile(const string& name);

	virtual bool SaveToFile(const string& name);

	virtual ISettingsValue& Get(const string& paramName);

	virtual int GetInteger(const string& paramName);

	virtual double GetFloat(const string& paramName);

	virtual bool GetBoolean(const string& paramName);

	virtual string GetString(const string& paramName);

	virtual void SetValue(const string& paramName, ISettingsValue& value);

	virtual void SetValue(const string& paramName, int value);

	virtual void SetValue(const string& paramName, double value);

	virtual void SetValue(const string& paramName, bool value);

	virtual void SetString(const string& paramName, const string& value);

	virtual void SetValue(const string& paramName, const char* value);

	virtual void SetInteger(const string& paramName, int value);

	virtual void SetFloat(const string& paramName, double value);

	virtual void SetBoolean(const string& paramName, bool value);

	virtual ~Settings();
};