#include "LuaInterface.h"
#include <iostream>
using namespace std;

//Allocating and initializing CLuaInterface's static data member
//The pointer is allocated but not the object's constructor
CLuaInterface *CLuaInterface::s_instance = 0;

CLuaInterface::CLuaInterface()
	: theLuaState(NULL)
{

}

CLuaInterface::~CLuaInterface()
{

}

//Initialisation of the Lua Interface Class
bool CLuaInterface::Init()
{
	bool result = false;
	//1. Create lua state
	theLuaState = lua_open();

	if (theLuaState)
	{
		//2. Load lua auxiliary libraries
		luaL_openlibs(theLuaState);

		//3.Load lua script
		luaL_dofile(theLuaState, "Image/DM2240.lua");

		result = true;
	}

	theErrorState = lua_open();
	if ((theLuaState) && (theErrorState))
	{
		//2. Load lua auxiliary libraries
		luaL_openlibs(theLuaState);

		//Load the error lua script
		luaL_dofile(theErrorState, "Image//errorLookup.lua");
	}

	return result;
}

//Run the Lua Interface Class
void CLuaInterface::Run()
{
	if (theLuaState == NULL)
		return;

	//4. Read the variables
	//lua_getglobal(lua_State*, const char*);
	lua_getglobal(theLuaState, "title");
	//extract value, index -1 as variable is already retrieved using lua_getglobal
	const char *title = lua_tostring(theLuaState, -1);

	lua_getglobal(theLuaState, "width");
	int width = lua_tointeger(theLuaState, -1);

	lua_getglobal(theLuaState, "height");
	int height = lua_tointeger(theLuaState, -1);

	//Display on screen
	cout << title << endl;
	cout << "-----------------------------------------" << endl;
	cout << "Width of screen : " << width << endl;
	cout << "Height of screen : " << height << endl;
}

//Drop the Lua Interface Class
void CLuaInterface::Drop()
{
	if (theLuaState)
	{
		//Close lua state
		lua_close(theLuaState);
	}
}

int CLuaInterface::getIntValue(const char* name,const char* fileName)
{
	luaL_dofile(theLuaState, fileName);
	lua_getglobal(theLuaState, name);
	luaL_dofile(theLuaState, "Image//DM2240.lua");
	return lua_tointeger(theLuaState, -1);
}

//void CLuaInterface::saveIntValue(const char * name, int value, bool overwrite)
//{
//	lua_getglobal(theLuaState, "SaveToLuaFile");
//	if (!lua_isfunction(theLuaState, -1))
//	{
//		lua_pop(theLuaState, 1);
//		return;
//	}
//	char interger_string[32];
//	sprintf(interger_string, "%d", value);
//	strcat(name, " = ");
//
//	lua_pushstring(theLuaState, name);
//	lua_pushinteger(theLuaState, overwrite);
//	lua_pcall(theLuaState, 2, 1, 0);
//}

// Save an integer value through the Lua Interface Class
void CLuaInterface::saveIntValue(const char* varName,
	const int value, const char* fileName, const bool bOverwrite)
{
	lua_getglobal(theLuaState, "SaveToLuaFile");
	char outputString[80];
	sprintf(outputString, "%s = %d\n", varName, value);
	lua_pushstring(theLuaState, outputString);
	lua_pushstring(theLuaState, fileName);
	lua_pushinteger(theLuaState, bOverwrite);
	lua_call(theLuaState, 3, 0); cout << "....................";
}
// Save a float value through the Lua Interface Class
void CLuaInterface::saveFloatValue(const char* varName,
	const float value, const char* fileName, const bool bOverwrite)
{
	lua_getglobal(theLuaState, "SaveToLuaFile");
	char outputString[80];
	sprintf(outputString, "%s = %6.4f\n", varName, value);
	lua_pushstring(theLuaState, outputString);
	lua_pushstring(theLuaState, fileName);
	lua_pushinteger(theLuaState, bOverwrite);
	lua_call(theLuaState, 3, 0);
}

void CLuaInterface::saveVector3Value(const char * varName, const Vector3 value,const char* fileName, const bool bOverwrite)
{
	lua_getglobal(theLuaState, "SaveToLuaFile");
	char outputString[80];
	sprintf(outputString, "%s = {%6.4f,%6.4f,%6.4f}\n", varName, value.x, value.y, value.z);
	lua_pushstring(theLuaState, outputString);
	lua_pushstring(theLuaState, fileName);
	lua_pushinteger(theLuaState, bOverwrite);
	lua_call(theLuaState, 3, 0);
}

void CLuaInterface::saveCharValue(const char * varName, const char value, const char * fileName, const bool bOverwrite)
{
	lua_getglobal(theLuaState, "SaveToLuaFile");
	char outputString[80];
	sprintf(outputString, "%s = %c\n", varName, value);
	lua_pushstring(theLuaState, outputString);
	lua_pushstring(theLuaState, fileName);
	lua_pushinteger(theLuaState, bOverwrite);
	lua_call(theLuaState, 3, 0);
}

void CLuaInterface::updateResolution(const char * varName, const int value, const int lineNumber)
{
	lua_getglobal(theLuaState, "UpdateLineInFile");
	char outputString[80];
	sprintf(outputString, "%s = %d", varName, value);
	lua_pushstring(theLuaState, outputString);
	lua_pushstring(theLuaState, "Image//DM2240.lua");
	lua_pushinteger(theLuaState, lineNumber);
	lua_call(theLuaState, 3, 0); cout << "....................";
}

char CLuaInterface::getCharValue(const char * varName)
{
	lua_getglobal(theLuaState, varName);

	size_t len;
	const char* cstr = lua_tolstring(theLuaState, -1, &len);
	// if the string is not empty, then return the first char
	if (len > 0)
		return cstr[0];
	else
		return ' ';
}

Vector3 CLuaInterface::getVector3Values(const char * varName,const char* fileName)
{
	luaL_dofile(theLuaState, fileName);

	lua_getglobal(theLuaState, varName);
	lua_rawgeti(theLuaState, -1, 1);
	float x = lua_tonumber(theLuaState, -1);
	lua_pop(theLuaState, 1);
	lua_rawgeti(theLuaState, -1, 2);
	float y = lua_tonumber(theLuaState, -1);
	lua_pop(theLuaState, 1);
	lua_rawgeti(theLuaState, -1, 3);
	float z = lua_tonumber(theLuaState, -1);
	lua_pop(theLuaState, 1);

	luaL_dofile(theLuaState, "Image//DM2240.lua");
	return Vector3(x, y, z);
}

float CLuaInterface::getDistanceSquareValue(const char * varName, Vector3 source, Vector3 destination)
{
	lua_getglobal(theLuaState, varName);
	lua_pushnumber(theLuaState, source.x);
	lua_pushnumber(theLuaState, source.y);
	lua_pushnumber(theLuaState, source.z);
	lua_pushnumber(theLuaState, destination.x);
	lua_pushnumber(theLuaState, destination.y);
	lua_pushnumber(theLuaState, destination.z);
	lua_call(theLuaState, 6, 1);
	float distanceSquare = (float)lua_tonumber(theLuaState, -1);
	lua_pop(theLuaState, 1);
	return distanceSquare;
}

int CLuaInterface::getVariableValues(const char * varName, int & a, int & b, int & c, int & d)
{
	lua_getglobal(theLuaState, varName);
	lua_pushnumber(theLuaState, 1000);
	lua_pushnumber(theLuaState, 2000);
	lua_pushnumber(theLuaState, 3000);
	lua_pushnumber(theLuaState, 4000);
	lua_call(theLuaState, 4, 4);
	a = lua_tonumber(theLuaState, -1);
	lua_pop(theLuaState, 1);
	b = lua_tonumber(theLuaState, -1);
	lua_pop(theLuaState, 1);
	c = lua_tonumber(theLuaState, -1);
	lua_pop(theLuaState, 1);
	d = lua_tonumber(theLuaState, -1);
	lua_pop(theLuaState, 1);

	return true;
}

float CLuaInterface::GetField(const char * key)
{
	int result = false;

	//Check if the variables in the Lua stack belongs to a table
	if (!lua_istable(theLuaState, -1))
		error("error100");

	lua_pushstring(theLuaState, key);
	lua_gettable(theLuaState, -2);
	if (!lua_isnumber(theLuaState, -1))
		error("error101");
	result = (int)lua_tonumber(theLuaState, -1);
	lua_pop(theLuaState, 1);
	return result;
}

void CLuaInterface::error(const char * errorCode)
{
	if (theErrorState == NULL);
	return;

	lua_getglobal(theErrorState, errorCode);
	const char *errorMsg = lua_tostring(theErrorState, -1);
	if (errorMsg != NULL)
		cout << errorMsg << endl;
	else
		cout << errorCode << " is not a vlid. \n*** Please contact the developer ***" << endl;
}

void CLuaInterface::replaceForward(const char * varName, const char value, const int lineNumber)
{
	//lua_getglobal(theLuaState, varName);
	//lua_pushnumber(theLuaState, value);
	//lua_call(theLuaState, 1, 1);
	////float distanceSquare = (float)lua_tonumber(theLuaState, -1);
	//lua_pop(theLuaState, 1);

	lua_getglobal(theLuaState, "UpdateLineInFile");
	char outputString[80];
	sprintf(outputString, "%s = \"%c\"", varName, value);
	lua_pushstring(theLuaState, outputString);
	lua_pushstring(theLuaState, "Image//DM2240.lua");
	lua_pushinteger(theLuaState, lineNumber);
	lua_call(theLuaState, 3, 0); cout << "....................";
}

std::string CLuaInterface::changeState(const char * currentState, int distanceFromPlayer)
{
	lua_getglobal(theLuaState, "ChangeEnemyState");
	lua_pushstring(theLuaState, currentState);
	lua_pushinteger(theLuaState, distanceFromPlayer);
	lua_call(theLuaState, 2, 1);
	std::string state = lua_tostring(theLuaState, -1);
	lua_pop(theLuaState, 1);
	return state;
}