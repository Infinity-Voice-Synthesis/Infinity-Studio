#include "ILLibs.h"
#include "utils/Config.h"

extern "C" {
#include "Lua/lstate.h"
}

std::function<void(juce::String&)> ILLibs::console_mesFunction;
std::function<void(juce::String&)> ILLibs::console_errFunction;
std::function<void()> ILLibs::console_clsFunction;

std::function<bool(juce::String&)> ILLibs::thread_finFunction;
std::function<juce::StringArray()> ILLibs::thread_lstFunction;
std::function<bool(juce::String&)> ILLibs::thread_creFunction;
std::function<bool(juce::String&)> ILLibs::thread_rmvFunction;
std::function<bool(juce::String&)> ILLibs::thread_chkFunction;
std::function<bool(juce::String&)> ILLibs::thread_desFunction;
std::function<bool(juce::String&, juce::String&)> ILLibs::thread_runFunction;
std::function<bool(juce::String&, juce::String&)> ILLibs::thread_execFunction;
std::function<void()> ILLibs::thread_fluFunction;

std::function<void* (juce::String&, juce::String&, size_t)> ILLibs::share_creFunction;
std::function<bool(juce::String&, juce::String&)> ILLibs::share_finFunction;
std::function<bool(juce::String&, juce::String&)> ILLibs::share_rmvFunction;
std::function<size_t(juce::String&, juce::String&)> ILLibs::share_sizFunction;
std::function<void* (juce::String&, juce::String&)> ILLibs::share_getFunction;
std::function<bool(juce::String&)> ILLibs::share_cleFunction;
std::function<juce::StringArray(juce::String&)> ILLibs::share_lstFunction;
std::function<void(juce::String&)> ILLibs::share_lckFunction;
std::function<void(juce::String&)> ILLibs::share_ulkFunction;

void ILLibs::reg_mesFunctions(
	std::function<void(juce::String&)> console_mesFunction,
	std::function<void(juce::String&)> console_errFunction,
	std::function<void()> console_clsFunction
)
{
	ILLibs::console_mesFunction = console_mesFunction;
	ILLibs::console_errFunction = console_errFunction;
	ILLibs::console_clsFunction = console_clsFunction;
}

void ILLibs::reg_thrFunctions(
	std::function<bool(juce::String&)> thread_finFunction,
	std::function<juce::StringArray()> thread_lstFunction,
	std::function<bool(juce::String&)> thread_creFunction,
	std::function<bool(juce::String&)> thread_rmvFunction,
	std::function<bool(juce::String&)> thread_chkFunction,
	std::function<bool(juce::String&)> thread_desFunction,
	std::function<bool(juce::String&, juce::String&)> thread_doFunction,
	std::function<bool(juce::String&, juce::String&)> thread_execFunction,
	std::function<void()> thread_fluFunction
)
{
	ILLibs::thread_finFunction = thread_finFunction;
	ILLibs::thread_lstFunction = thread_lstFunction;
	ILLibs::thread_creFunction = thread_creFunction;
	ILLibs::thread_rmvFunction = thread_rmvFunction;
	ILLibs::thread_chkFunction = thread_chkFunction;
	ILLibs::thread_desFunction = thread_desFunction;
	ILLibs::thread_runFunction = thread_doFunction;
	ILLibs::thread_execFunction = thread_execFunction;
	ILLibs::thread_fluFunction = thread_fluFunction;
}

void ILLibs::reg_shrFunctions(
	std::function<void* (juce::String&, juce::String&, size_t)> share_creFunction,
	std::function<bool(juce::String&, juce::String&)> share_finFunction,
	std::function<bool(juce::String&, juce::String&)> share_rmvFunction,
	std::function<size_t(juce::String&, juce::String&)> share_sizFunction,
	std::function<void* (juce::String&, juce::String&)> share_getFunction,
	std::function<bool(juce::String&)> share_cleFunction,
	std::function<juce::StringArray(juce::String&)> share_lstFunction,
	std::function<void(juce::String&)> share_lckFunction,
	std::function<void(juce::String&)> share_ulkFunction
)
{
	ILLibs::share_creFunction = share_creFunction;
	ILLibs::share_finFunction = share_finFunction;
	ILLibs::share_rmvFunction = share_rmvFunction;
	ILLibs::share_sizFunction = share_sizFunction;
	ILLibs::share_getFunction = share_getFunction;
	ILLibs::share_cleFunction = share_cleFunction;
	ILLibs::share_lstFunction = share_lstFunction;
	ILLibs::share_lckFunction = share_lckFunction;
	ILLibs::share_ulkFunction = share_ulkFunction;
}

int ILLibs::infinity_runtime_scriptPath(lua_State* state)
{
	std::string str = luaL_checkstring(state, 1);
	juce::String name = juce::String::createStringFromData(str.c_str(), str.size());
	lua_pushstring(state, Config::script(name).toStdString().c_str());
	return 1;
}

int ILLibs::infinity_runtime_scriptDir(lua_State* state)
{
	lua_pushstring(state, Config::scriptDir().toStdString().c_str());
	return 1;
}

int ILLibs::infinity_runtime_appPath(lua_State* state)
{
	lua_pushstring(state, Config::appPath().toStdString().c_str());
	return 1;
}

int ILLibs::infinity_runtime_appDir(lua_State* state)
{
	lua_pushstring(state, Config::appDir().toStdString().c_str());
	return 1;
}

int ILLibs::infinity_runtime_msleep(lua_State* state)
{
	int msecs = luaL_checkinteger(state, 1);
	juce::Time::waitForMillisecondCounter(msecs);
	return 0;
}

int ILLibs::infinity_console_println(lua_State* state)
{
	std::string str = luaL_checkstring(state, 1);
	juce::String message = juce::String::createStringFromData(str.c_str(), str.size());
	if (!message.isEmpty()) {
		ILLibs::console_mesFunction(message);
	}
	return 0;
}

int ILLibs::infinity_console_error(lua_State* state)
{
	std::string str = luaL_checkstring(state, 1);
	juce::String message = juce::String::createStringFromData(str.c_str(), str.size());
	if (!message.isEmpty()) {
		ILLibs::console_errFunction(message);
	}
	return 0;
}

int ILLibs::infinity_console_cls(lua_State* state)
{
	ILLibs::console_clsFunction();
	return 0;
}

int ILLibs::infinity_thread_current(lua_State* state)
{
	juce::ReadWriteLock* mutex = reinterpret_cast<juce::ReadWriteLock*>(state->thread_mutex);
	juce::String* pStr = (juce::String*)state->thread_id;
	mutex->enterRead();
	juce::String tName = (*pStr);
	mutex->exitRead();
	lua_pushstring(state, tName.toStdString().c_str());
	return 1;
}

int ILLibs::infinity_thread_find(lua_State* state)
{
	std::string str = luaL_checkstring(state, 1);
	juce::String id = juce::String::createStringFromData(str.c_str(), str.size());
	lua_pushboolean(state, ILLibs::thread_finFunction(id));
	return 1;
}

int ILLibs::infinity_thread_list(lua_State* state)
{
	juce::StringArray tList = ILLibs::thread_lstFunction();
	lua_newtable(state);
	int count = 0;
	for (auto& s : tList) {
		lua_pushnumber(state, ++count);
		lua_pushstring(state, s.toStdString().c_str());
		lua_settable(state, -3);
	}
	return 1;
}

int ILLibs::infinity_thread_create(lua_State* state)
{
	std::string str = luaL_checkstring(state, 1);
	juce::String id = juce::String::createStringFromData(str.c_str(), str.size());
	bool ok = ILLibs::thread_creFunction(id);
	if (!ok) {
		juce::String error = "Can't create thread:" + id;
		ILLibs::console_errFunction(error);
	}
	lua_pushboolean(state, ok);
	return 1;
}

int ILLibs::infinity_thread_remove(lua_State* state)
{
	std::string str = luaL_checkstring(state, 1);
	juce::String id = juce::String::createStringFromData(str.c_str(),str.size());
	bool ok = ILLibs::thread_rmvFunction(id);
	if (!ok) {
		juce::String error = "Can't remove thread:" + id;
		ILLibs::console_errFunction(error);
	}
	lua_pushboolean(state, ok);
	return 1;
}

int ILLibs::infinity_thread_destory(lua_State* state)
{
	std::string str = luaL_checkstring(state, 1);
	juce::String id = juce::String::createStringFromData(str.c_str(), str.size());
	juce::ReadWriteLock* mutex = reinterpret_cast<juce::ReadWriteLock*>(state->thread_mutex);
	juce::String* pStr = (juce::String*)state->thread_id;
	mutex->enterRead();
	juce::String tName = (*pStr);
	mutex->exitRead();
	if (id == tName) {
		juce::String error = "The thread can't destory itself!";
		ILLibs::console_errFunction(error);
		return 0;
	}
	bool ok = ILLibs::thread_desFunction(id);
	if (!ok) {
		juce::String error = "Can't destory thread:" + id;
		ILLibs::console_errFunction(error);
	}
	lua_pushboolean(state, ok);
	return 1;
}

int ILLibs::infinity_thread_check(lua_State* state)
{
	std::string str = luaL_checkstring(state, 1);
	juce::String id = juce::String::createStringFromData(str.c_str(), str.size());
	lua_pushboolean(state, ILLibs::thread_chkFunction(id));
	return 1;
}

int ILLibs::infinity_thread_run(lua_State* state)
{
	std::string str1 = luaL_checkstring(state, 1);
	std::string str2 = luaL_checkstring(state, 2);
	juce::String id = juce::String::createStringFromData(str1.c_str(), str1.size());
	juce::String str = juce::String::createStringFromData(str2.c_str(), str2.size());
	bool ok = ILLibs::thread_runFunction(id, str);
	if (!ok) {
		juce::String error = "Can't run \"" + str + "\" on thread:" + id;
		ILLibs::console_errFunction(error);
	}
	lua_pushboolean(state, ok);
	return 1;
}

int ILLibs::infinity_thread_exec(lua_State* state)
{
	std::string str1 = luaL_checkstring(state, 1);
	std::string str2 = luaL_checkstring(state, 2);
	juce::String id = juce::String::createStringFromData(str1.c_str(), str1.size());
	juce::String str = juce::String::createStringFromData(str2.c_str(), str2.size());
	bool ok = ILLibs::thread_execFunction(id, str);
	if (!ok) {
		juce::String error = "Can't execute \"" + str + "\" on thread:" + id;
		ILLibs::console_errFunction(error);
	}
	lua_pushboolean(state, ok);
	return 1;
}

int ILLibs::infinity_thread_flush(lua_State* state)
{
	ILLibs::thread_fluFunction();
	return 0;
}

int ILLibs::infinity_thread_share_create(lua_State* state)
{
	std::string str1 = luaL_checkstring(state, 1);
	std::string str2 = luaL_checkstring(state, 2);
	juce::String id = juce::String::createStringFromData(str1.c_str(), str1.size());
	juce::String key = juce::String::createStringFromData(str2.c_str(), str2.size());
	size_t size = luaL_checkinteger(state, 3);
	void* result = ILLibs::share_creFunction(id, key, size);
	if (result == nullptr) {
		juce::String error = "Can't create share data \"" + key + "\" on thread:" + id;
		ILLibs::console_errFunction(error);
	}
	lua_pushlightuserdata(state, result);
	return 1;
}

int ILLibs::infinity_thread_share_find(lua_State* state)
{
	std::string str1 = luaL_checkstring(state, 1);
	std::string str2 = luaL_checkstring(state, 2);
	juce::String id = juce::String::createStringFromData(str1.c_str(), str1.size());
	juce::String key = juce::String::createStringFromData(str2.c_str(), str2.size());
	lua_pushboolean(state, ILLibs::share_finFunction(id, key));
	return 1;
}

int ILLibs::infinity_thread_share_remove(lua_State* state)
{
	std::string str1 = luaL_checkstring(state, 1);
	std::string str2 = luaL_checkstring(state, 2);
	juce::String id = juce::String::createStringFromData(str1.c_str(), str1.size());
	juce::String key = juce::String::createStringFromData(str2.c_str(), str2.size());
	bool ok = ILLibs::share_rmvFunction(id, key);
	if (!ok) {
		juce::String error = "Can't remove share data \"" + key + "\" on thread:" + id;
		ILLibs::console_errFunction(error);
	}
	lua_pushboolean(state, ok);
	return 1;
}

int ILLibs::infinity_thread_share_size(lua_State* state)
{
	std::string str1 = luaL_checkstring(state, 1);
	std::string str2 = luaL_checkstring(state, 2);
	juce::String id = juce::String::createStringFromData(str1.c_str(), str1.size());
	juce::String key = juce::String::createStringFromData(str2.c_str(), str2.size());
	lua_pushinteger(state, ILLibs::share_sizFunction(id, key));
	return 1;
}

int ILLibs::infinity_thread_share_get(lua_State* state)
{
	std::string str1 = luaL_checkstring(state, 1);
	std::string str2 = luaL_checkstring(state, 2);
	juce::String id = juce::String::createStringFromData(str1.c_str(), str1.size());
	juce::String key = juce::String::createStringFromData(str2.c_str(), str2.size());
	void* result = ILLibs::share_getFunction(id, key);
	if (result == nullptr) {
		juce::String error = "Can't get share data \"" + key + "\" on thread:%s" + id;
		ILLibs::console_errFunction(error);
	}
	lua_pushlightuserdata(state, result);
	return 1;
}

int ILLibs::infinity_thread_share_clear(lua_State* state)
{
	std::string str = luaL_checkstring(state, 1);
	juce::String id = juce::String::createStringFromData(str.c_str(), str.size());
	bool ok = ILLibs::share_cleFunction(id);
	if (!ok) {
		juce::String error = "Can't clear share data on thread:" + id;
		ILLibs::console_errFunction(error);
	}
	lua_pushboolean(state, ok);
	return 1;
}

int ILLibs::infinity_thread_share_list(lua_State* state)
{
	std::string str = luaL_checkstring(state, 1);
	juce::String id = juce::String::createStringFromData(str.c_str(), str.size());
	juce::StringArray tList = ILLibs::share_lstFunction(id);
	lua_newtable(state);
	int count = 0;
	for (auto& s : tList) {
		lua_pushnumber(state, ++count);
		lua_pushstring(state, s.toStdString().c_str());
		lua_settable(state, -3);
	}
	return 1;
}

int ILLibs::infinity_thread_share_lock(lua_State* state)
{
	std::string str = luaL_checkstring(state, 1);
	juce::String id = juce::String::createStringFromData(str.c_str(), str.size());
	ILLibs::share_lckFunction(id);
	return 0;
}

int ILLibs::infinity_thread_share_unlock(lua_State* state)
{
	std::string str = luaL_checkstring(state, 1);
	juce::String id = juce::String::createStringFromData(str.c_str(), str.size());
	ILLibs::share_ulkFunction(id);
	return 0;
}
