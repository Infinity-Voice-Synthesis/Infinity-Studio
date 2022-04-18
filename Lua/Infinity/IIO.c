#include "IIO.h"
#include <stdbool.h>

static LUA_InfOChar_Function LUA_InfOChar;
static LUA_InfOLine_Function LUA_InfOLine;
static LUA_InfOError_Function LUA_InfOError;

void set_LUA_InfOChar(LUA_InfOChar_Function arg)
{
	LUA_InfOChar = arg;
}

void set_LUA_InfOLine(LUA_InfOLine_Function arg)
{
	LUA_InfOLine = arg;
}

void set_LUA_InfOError(LUA_InfOError_Function arg)
{
	LUA_InfOError = arg;
}

LUA_InfOChar_Function get_LUA_InfOChar()
{
	return LUA_InfOChar;
}

LUA_InfOLine_Function get_LUA_InfOLine()
{
	return LUA_InfOLine;
}

LUA_InfOError_Function get_LUA_InfOError()
{
	return LUA_InfOError;
}

#define LOCK_ENABLE 0

static bool OChar_Locked = false;

void LUA_OChar_Lock()
{
#if LOCK_ENABLE
	while (OChar_Locked);
	OChar_Locked = true;
#endif
}

void LUA_OChar_Unlock()
{
#if LOCK_ENABLE
	OChar_Locked = false;
#endif
}