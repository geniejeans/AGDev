#pragma once

//Includes the lua headers
#include "lua.hpp"

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

	int getIntValue(const char* name);
//	void saveIntValue(const char* name, int value, bool overwrite = false);
//	void saveFloatValue(const char* name, float value, bool smth = false);

	// Save an integer value through the Lua Interface Class
	void saveIntValue(const char* varName, const int value, const bool bOverwrite = NULL);
	// Save a float value through the Lua Interface Class
	void saveFloatValue(const char* varName, const float value, const bool bOverwrite = NULL);
};