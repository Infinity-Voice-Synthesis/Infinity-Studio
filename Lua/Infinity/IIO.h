#pragma once

#include <stdlib.h>
#include "../lua.h"

typedef void(*LUA_InfOChar_Function)(lua_State*, const char*, size_t);
typedef void(*LUA_InfOLine_Function)(lua_State*);
typedef void(*LUA_InfOError_Function)(lua_State*, const char*, const char*);


void set_LUA_InfOChar(LUA_InfOChar_Function arg);

void set_LUA_InfOLine(LUA_InfOLine_Function arg);

void set_LUA_InfOError(LUA_InfOError_Function arg);

LUA_InfOChar_Function get_LUA_InfOChar();

LUA_InfOLine_Function get_LUA_InfOLine();

LUA_InfOError_Function get_LUA_InfOError();
