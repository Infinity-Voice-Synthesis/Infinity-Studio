#include "ILLibs.h"

extern "C" {
#include "Lua/lstate.h"
}

std::function<void(QString&)> ILLibs::console_mesFunction;
std::function<void(QString&)> ILLibs::console_errFunction;
std::function<void()> ILLibs::console_clsFunction;

std::function<bool(QString&)> ILLibs::thread_finFunction;
std::function<QStringList()> ILLibs::thread_lstFunction;
std::function<bool(QString&)> ILLibs::thread_creFunction;
std::function<bool(QString&)> ILLibs::thread_rmvFunction;
std::function<bool(QString&)> ILLibs::thread_chkFunction;
std::function<bool(QString&)> ILLibs::thread_desFunction;
std::function<bool(QString&, QString&)> ILLibs::thread_runFunction;
std::function<bool(QString&, QString&)> ILLibs::thread_execFunction;
std::function<void()> ILLibs::thread_fluFunction;

std::function<void* (QString&, QString&, size_t)> ILLibs::share_creFunction;
std::function<bool(QString&, QString&)> ILLibs::share_finFunction;
std::function<bool(QString&, QString&)> ILLibs::share_rmvFunction;
std::function<size_t(QString&, QString&)> ILLibs::share_sizFunction;
std::function<void* (QString&, QString&)> ILLibs::share_getFunction;
std::function<bool(QString&)> ILLibs::share_cleFunction;
std::function<QStringList(QString&)> ILLibs::share_lstFunction;
std::function<void(QString&)> ILLibs::share_lckFunction;
std::function<void(QString&)> ILLibs::share_ulkFunction;

void ILLibs::reg_mesFunctions(
	std::function<void(QString&)> console_mesFunction,
	std::function<void(QString&)> console_errFunction,
	std::function<void()> console_clsFunction
)
{
	ILLibs::console_mesFunction = console_mesFunction;
	ILLibs::console_errFunction = console_errFunction;
	ILLibs::console_clsFunction = console_clsFunction;
}

void ILLibs::reg_thrFunctions(
	std::function<bool(QString&)> thread_finFunction,
	std::function<QStringList()> thread_lstFunction,
	std::function<bool(QString&)> thread_creFunction,
	std::function<bool(QString&)> thread_rmvFunction,
	std::function<bool(QString&)> thread_chkFunction,
	std::function<bool(QString&)> thread_desFunction,
	std::function<bool(QString&, QString&)> thread_doFunction,
	std::function<bool(QString&, QString&)> thread_execFunction,
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
	std::function<void* (QString&, QString&, size_t)> share_creFunction,
	std::function<bool(QString&, QString&)> share_finFunction,
	std::function<bool(QString&, QString&)> share_rmvFunction,
	std::function<size_t(QString&, QString&)> share_sizFunction,
	std::function<void* (QString&, QString&)> share_getFunction,
	std::function<bool(QString&)> share_cleFunction,
	std::function<QStringList(QString&)> share_lstFunction,
	std::function<void(QString&)> share_lckFunction,
	std::function<void(QString&)> share_ulkFunction
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
	QString name = QString::fromStdString(luaL_checkstring(state, 1));
	lua_pushstring(state, QString(QCoreApplication::applicationDirPath() + "/scripts/" + name + ".lua").toStdString().c_str());
	return 1;
}

int ILLibs::infinity_runtime_scriptDir(lua_State* state)
{
	lua_pushstring(state, QString(QCoreApplication::applicationDirPath() + "/scripts").toStdString().c_str());
	return 1;
}

int ILLibs::infinity_runtime_appPath(lua_State* state)
{
	lua_pushstring(state, QString(QCoreApplication::applicationFilePath()).toStdString().c_str());
	return 1;
}

int ILLibs::infinity_runtime_appDir(lua_State* state)
{
	lua_pushstring(state, QString(QCoreApplication::applicationDirPath()).toStdString().c_str());
	return 1;
}

int ILLibs::infinity_runtime_msleep(lua_State* state)
{
	int msecs = luaL_checkinteger(state, 1);
	QTime timePoint = QTime::currentTime();
	while (timePoint.msecsTo(QTime::currentTime()) < msecs);
	return 0;
}

int ILLibs::infinity_console_println(lua_State* state)
{
	QString message = QString::fromStdString(luaL_checkstring(state, 1));
	if (!message.isEmpty()) {
		ILLibs::console_mesFunction(message);
	}
	return 0;
}

int ILLibs::infinity_console_error(lua_State* state)
{
	QString message = QString::fromStdString(luaL_checkstring(state, 1));
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
	QMutex* mutex = (QMutex*)state->thread_mutex;
	QString* str = (QString*)state->thread_id;
	mutex->lock();
	QString tName = (*str);
	mutex->unlock();
	lua_pushstring(state, tName.toStdString().c_str());
	return 1;
}

int ILLibs::infinity_thread_find(lua_State* state)
{
	QString id = QString::fromStdString(luaL_checkstring(state, 1));
	lua_pushboolean(state, ILLibs::thread_finFunction(id));
	return 1;
}

int ILLibs::infinity_thread_list(lua_State* state)
{
	QStringList tList = ILLibs::thread_lstFunction();
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
	QString id = QString::fromStdString(luaL_checkstring(state, 1));
	bool ok = ILLibs::thread_creFunction(id);
	if (!ok) {
		QString error = QString::asprintf("Can't create thread:%s", qPrintable(id));
		ILLibs::console_errFunction(error);
	}
	lua_pushboolean(state, ok);
	return 1;
}

int ILLibs::infinity_thread_remove(lua_State* state)
{
	QString id = QString::fromStdString(luaL_checkstring(state, 1));
	bool ok = ILLibs::thread_rmvFunction(id);
	if (!ok) {
		QString error = QString::asprintf("Can't remove thread:%s", qPrintable(id));
		ILLibs::console_errFunction(error);
	}
	lua_pushboolean(state, ok);
	return 1;
}

int ILLibs::infinity_thread_destory(lua_State* state)
{
	QString id = QString::fromStdString(luaL_checkstring(state, 1));
	QMutex* mutex = (QMutex*)state->thread_mutex;
	QString* str = (QString*)state->thread_id;
	mutex->lock();
	QString tName = (*str);
	mutex->unlock();
	if (id == tName) {
		QString error = "The thread can't destory itself!";
		ILLibs::console_errFunction(error);
		return 0;
	}
	bool ok = ILLibs::thread_desFunction(id);
	if (!ok) {
		QString error = QString::asprintf("Can't destory thread:%s", qPrintable(id));
		ILLibs::console_errFunction(error);
	}
	lua_pushboolean(state, ok);
	return 1;
}

int ILLibs::infinity_thread_check(lua_State* state)
{
	QString id = QString::fromStdString(luaL_checkstring(state, 1));
	lua_pushboolean(state, ILLibs::thread_chkFunction(id));
	return 1;
}

int ILLibs::infinity_thread_run(lua_State* state)
{
	QString id = QString::fromStdString(luaL_checkstring(state, 1));
	QString str = QString::fromStdString(luaL_checkstring(state, 2));
	bool ok = ILLibs::thread_runFunction(id, str);
	if (!ok) {
		QString error = QString::asprintf("Can't run \"%s\" on thread:%s", qPrintable(str), qPrintable(id));
		ILLibs::console_errFunction(error);
	}
	lua_pushboolean(state, ok);
	return 1;
}

int ILLibs::infinity_thread_exec(lua_State* state)
{
	QString id = QString::fromStdString(luaL_checkstring(state, 1));
	QString str = QString::fromStdString(luaL_checkstring(state, 2));
	bool ok = ILLibs::thread_execFunction(id, str);
	if (!ok) {
		QString error = QString::asprintf("Can't execute \"%s\" on thread:%s", qPrintable(str), qPrintable(id));
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
	QString id = QString::fromStdString(luaL_checkstring(state, 1));
	QString key = QString::fromStdString(luaL_checkstring(state, 2));
	size_t size = luaL_checkinteger(state, 3);
	void* result = ILLibs::share_creFunction(id, key, size);
	if (result == nullptr) {
		QString error = QString::asprintf("Can't create share data \"%s\" on thread:%s", qPrintable(key), qPrintable(id));
		ILLibs::console_errFunction(error);
	}
	lua_pushlightuserdata(state, result);
	return 1;
}

int ILLibs::infinity_thread_share_find(lua_State* state)
{
	QString id = QString::fromStdString(luaL_checkstring(state, 1));
	QString key = QString::fromStdString(luaL_checkstring(state, 2));
	lua_pushboolean(state, ILLibs::share_finFunction(id, key));
	return 1;
}

int ILLibs::infinity_thread_share_remove(lua_State* state)
{
	QString id = QString::fromStdString(luaL_checkstring(state, 1));
	QString key = QString::fromStdString(luaL_checkstring(state, 2));
	bool ok = ILLibs::share_rmvFunction(id, key);
	if (!ok) {
		QString error = QString::asprintf("Can't remove share data \"%s\" on thread:%s", qPrintable(key), qPrintable(id));
		ILLibs::console_errFunction(error);
	}
	lua_pushboolean(state, ok);
	return 1;
}

int ILLibs::infinity_thread_share_size(lua_State* state)
{
	QString id = QString::fromStdString(luaL_checkstring(state, 1));
	QString key = QString::fromStdString(luaL_checkstring(state, 2));
	lua_pushinteger(state, ILLibs::share_sizFunction(id, key));
	return 1;
}

int ILLibs::infinity_thread_share_get(lua_State* state)
{
	QString id = QString::fromStdString(luaL_checkstring(state, 1));
	QString key = QString::fromStdString(luaL_checkstring(state, 2));
	void* result = ILLibs::share_getFunction(id, key);
	if (result == nullptr) {
		QString error = QString::asprintf("Can't get share data \"%s\" on thread:%s", qPrintable(key), qPrintable(id));
		ILLibs::console_errFunction(error);
	}
	lua_pushlightuserdata(state, result);
	return 1;
}

int ILLibs::infinity_thread_share_clear(lua_State* state)
{
	QString id = QString::fromStdString(luaL_checkstring(state, 1));
	bool ok = ILLibs::share_cleFunction(id);
	if (!ok) {
		QString error = QString::asprintf("Can't clear share data on thread:%s", qPrintable(id));
		ILLibs::console_errFunction(error);
	}
	lua_pushboolean(state, ok);
	return 1;
}

int ILLibs::infinity_thread_share_list(lua_State* state)
{
	QString id = QString::fromStdString(luaL_checkstring(state, 1));
	QStringList tList = ILLibs::share_lstFunction(id);
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
	QString id = QString::fromStdString(luaL_checkstring(state, 1));
	ILLibs::share_lckFunction(id);
	return 0;
}

int ILLibs::infinity_thread_share_unlock(lua_State* state)
{
	QString id = QString::fromStdString(luaL_checkstring(state, 1));
	ILLibs::share_ulkFunction(id);
	return 0;
}
