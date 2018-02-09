#pragma once

//Includes the lua headers
#include "lua.hpp"
#include "Vector3.h"

class CLuaInterface
{
protected:
	static CLuaInterface *s_instance;
	CLuaInterface();
public:
	static CLuaInterface *GetInstance()
	{
		if (!s_instance)
			s_instance = new CLuaInterface;
		return s_instance;
	}
	static bool DropInstance()
	{
		if (s_instance)
		{
			//Drop the Lua Interfance class
			s_instance->Drop();

			delete s_instance;
			s_instance = NULL;
			return true;
		}
		return false;
	}
	virtual ~CLuaInterface();

	//Initialisation of the Lua Interface Class
	bool Init();

	//Run the Lua Interface Class
	void Run();

	//Drop the Lua Interface Class
	void Drop();

	//Pointer to the Lua State
	lua_State *theLuaState;
	lua_State *theErrorState;

	int getIntValue(const char* name, const char* fileName);
//	void saveIntValue(const char* name, int value, bool overwrite = false);
//	void saveFloatValue(const char* name, float value, bool smth = false);

	// Save an integer value through the Lua Interface Class
	void saveIntValue(const char* varName, const int value, const char* fileName, const bool bOverwrite = NULL);
	// Save a float value through the Lua Interface Class
	void saveFloatValue(const char* varName, const float value, const char* fileName, const bool bOverwrite = NULL);
	// Save a vector3 value through the Lua Interface Class
	void saveVector3Value(const char* varName, const Vector3 value, const char* fileName,const bool bOverwrite = NULL);
	// Save a char value through the Lua Interface Class
	void saveCharValue(const char* varName, const char value, const char* fileName, const bool bOverwrite = NULL);
	//Update the file
	void updateResolution(const char* varName, const int value, const int lineNumber);
	// Get a char value through the Lua Interface Class
	char getCharValue(const char* varName);
	// Get Vector3 values through the Lua Interface Class
	Vector3 getVector3Values(const char* varName, const char* fileName);
	// Get distance square value through the Lua Interface Class
	float getDistanceSquareValue(const char* varName, Vector3 source, Vector3 destination);
	// Get variable number of values through the Lua Interface Class
	int getVariableValues(const char* varName, int &a, int &b, int &c, int &d);
	// Extract a field from a table
	float GetField(const char *key);
	// Get error message using an error cose
	void error(const char *errorCode);
	// replace forward button
	void replaceForward(const char* varName, const char value, const int lineNumber);
};