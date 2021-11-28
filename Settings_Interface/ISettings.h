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
public:
	virtual void SetValue(const string& value) = 0;

	virtual void SetValue(DataType type, const string& value) = 0;

	virtual string AsString() = 0;

	virtual int AsInteger() = 0;

	virtual double AsDouble() = 0;

	virtual bool AsBoolean() = 0;

	virtual DataType GetType() = 0;

	virtual ~ISettingsValue() {};
};

class ISettings
{
public:
	virtual bool LoadFromFile(const string& name) = 0;

	virtual bool SaveToFile(const string& name) = 0;

	virtual ISettingsValue& Get(const string& paramName) = 0;

	virtual int GetInteger(const string& paramName) = 0;

	virtual double GetFloat(const string& paramName) = 0;

	virtual bool GetBoolean(const string& paramName) = 0;

	virtual string GetString(const string& paramName) = 0;

	virtual void SetValue(const string& paramName, ISettingsValue& value) = 0;

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
