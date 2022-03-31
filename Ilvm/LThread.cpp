#include "LThread.h"
#include "utils/Config.h"

extern "C" {
#include "Lua/lstate.h"
}

juce::String LThread::destoryId;

std::function<void(const juce::String&)> LThread::errorMessage;
std::function<void(const juce::String&)> LThread::normalMessage;

std::function<void(const juce::String&)> LThread::tStarted;
std::function<void(const juce::String&)> LThread::tEnded;

void LThread::init(
	std::function<void(const juce::String&)> errorMessage,
	std::function<void(const juce::String&)> normalMessage,
	std::function<void(const juce::String&)> tStarted,
	std::function<void(const juce::String&)> tEnded
)
{
	LThread::errorMessage = errorMessage;
	LThread::normalMessage = normalMessage;
	LThread::tStarted = tStarted;
	LThread::tEnded = tEnded;
}

void LThread::set_destory(const juce::String& destoryId)
{
	LThread::destoryId = destoryId;
}

void LThread::hookFunction(lua_State* L, lua_Debug* ar)
{
	juce::ReadWriteLock* mutex = (juce::ReadWriteLock*)L->thread_mutex;
	juce::String* str = (juce::String*)L->thread_id;
	mutex->enterRead();
	juce::String tName = (*str);
	mutex->exitRead();
	if (tName == LThread::destoryId) {
		lua_pushstring(L, "This thread is destoried!");
		lua_error(L);
	}
}

LThread::LThread()
	: juce::Thread("LThread")
{
	this->lstate = luaL_newstate();
	this->lstate->thread_id = (void*)(&(this->Id));
	this->lstate->thread_mutex = (void*)(&(this->idMutex));

	lua_sethook(this->lstate, LThread::hookFunction, LUA_MASKLINE, 0);

	luaL_openlibs(this->lstate);

	bool error1 = luaL_dostring(this->lstate, juce::String("package.path = '" + Config::scriptDir() + "?.lua;;" + Config::scriptDir() + "?/?.lua;'").toStdString().c_str());
	if (error1) {
		//qDebug("Lua Error:%s", qPrintable(lua_tostring(this->lstate, -1)));
		lua_pop(this->lstate, 1);
	}
	bool error2 = luaL_dostring(this->lstate, juce::String("package.cpath = '" + Config::scriptDir() + "libs/?.dll;'").toStdString().c_str());
	if (error2) {
		//qDebug("Lua Error:%s", qPrintable(lua_tostring(this->lstate, -1)));
		lua_pop(this->lstate, 1);
	}
}

LThread::~LThread()
{
	lua_close(this->lstate);

	for (auto p : this->shareData) {
		free(p.second);
	}
	this->shareData.clear();
}

void LThread::run()
{
	this->tStarted(this->Id);
	if (this->tType == LType::DoFile) {
		bool error = luaL_dofile(this->lstate, this->lFileName.toStdString().c_str());
		if (error) {
			std::string str = lua_tostring(this->lstate, -1);
			this->errorMessage(juce::String::createStringFromData(str.c_str(), str.size()));
			lua_pop(this->lstate, 1);
			this->tEnded(this->Id);
			return;
		}
	}
	while (this->strList.size() > 0) {
		bool error = luaL_dostring(this->lstate, this->strList.front().toStdString().c_str());
		this->strList.pop();
		if (error) {
			std::string str = lua_tostring(this->lstate, -1);
			this->errorMessage(juce::String::createStringFromData(str.c_str(), str.size()));
			lua_pop(this->lstate, 1);
		}
	}
	this->tEnded(this->Id);
}

bool LThread::doFile(const juce::String& name)
{
	if (!this->Id.isEmpty()) {
		if (!this->isThreadRunning()) {
			this->tType = LType::DoFile;
			this->lFileName = name;
			this->startThread();
			return true;
		}
	}
	return false;
}

bool LThread::doString(const juce::String& str)
{
	if (!this->Id.isEmpty()) {
		this->tType = LType::DoString;
		this->strList.push(str);
		if (!this->isThreadRunning()) {
			this->startThread();
		}
		else {
			this->normalMessage("The VM is running.Command will wait in queue.");
		}
		return true;
	}
	return false;
}

bool LThread::setId(const juce::String& id)
{
	if (!id.isEmpty()) {
		if (this->Id.isEmpty()) {
			this->Id = id;
			return true;
		}
	}
	return false;
}

const juce::String& LThread::getId()
{
	return this->Id;
}

void LThread::beginGlobalTable()
{
	lua_newtable(this->lstate);
}

void LThread::endGlobalTable(const juce::String& name)
{
	lua_setglobal(this->lstate, name.toStdString().c_str());
}

void LThread::beginTable(const juce::String& name)
{
	lua_pushstring(this->lstate, name.toStdString().c_str());
	lua_newtable(this->lstate);
}

void LThread::endTable()
{
	lua_settable(this->lstate, -3);
}

void LThread::addFunction(const juce::String& name, lua_CFunction function)
{
	lua_pushstring(this->lstate, name.toStdString().c_str());
	lua_pushcfunction(this->lstate, function);
	lua_settable(this->lstate, -3);
}

void LThread::loadUtils()
{
	std::string comm("IUtils = require(\"utils\")");
	bool error = luaL_dostring(this->lstate, comm.c_str());
	if (error) {
		std::string str = lua_tostring(this->lstate, -1);
		this->errorMessage(juce::String::createStringFromData(str.c_str(), str.size()));
		lua_pop(this->lstate, 1);
	}
	error = luaL_dostring(this->lstate, "Infinity.Console.print = print");
	if (error) {
		std::string str = lua_tostring(this->lstate, -1);
		this->errorMessage(juce::String::createStringFromData(str.c_str(), str.size()));
		lua_pop(this->lstate, 1);
	}
}

bool LThread::destory(const juce::String& id)
{
	if (this->isThreadRunning()) {
		this->idMutex.enterWrite();
		this->setCurrentThreadName(id);
		this->Id = id;
		this->idMutex.exitWrite();
		return true;
	}
	return false;
}

bool LThread::checkShare(const juce::String& key)
{
	this->shareMutex.lock();
	bool result = this->shareData.contains(key);
	this->shareMutex.unlock();
	return result;
}

void* LThread::newShare(const juce::String& key, size_t size)
{
	if (checkShare(key)) {
		return nullptr;
	}
	this->shareMutex.lock();
	void* result = malloc(size);
	this->shareData.set(key, std::make_pair(size, result));
	this->shareMutex.unlock();
	return result;
}

bool LThread::removeShare(const juce::String& key)
{
	if (!checkShare(key)) {
		return false;
	}
	this->shareMutex.lock();
	free(this->shareData.getReference(key).second);
	this->shareData.remove(key);
	this->shareMutex.unlock();
	return true;
}

void* LThread::getShare(const juce::String& key)
{
	if (!checkShare(key)) {
		return nullptr;
	}
	this->shareMutex.lock();
	void* result = this->shareData.getReference(key).second;
	this->shareMutex.unlock();
	return result;
}

size_t LThread::sizeShare(const juce::String& key)
{
	if (!checkShare(key)) {
		return 0;
	}
	this->shareMutex.lock();
	size_t result = this->shareData.getReference(key).first;
	this->shareMutex.unlock();
	return result;
}

bool LThread::clearShare()
{
	this->shareMutex.lock();
	for (auto p : this->shareData) {
		if (p.second != nullptr) {
			free(p.second);
		}
	}
	this->shareData.clear();
	this->shareMutex.unlock();
	return true;
}

juce::StringArray LThread::listShare()
{
	this->shareMutex.lock();
	juce::StringArray result;
	for (auto it = this->shareData.begin(); it != this->shareData.end(); it.next()) {
		result.add(it.getKey());
	}
	this->shareMutex.unlock();
	return result;
}

void LThread::lockShare()
{
	this->shareMutex.lock();
}

void LThread::unlockShare()
{
#pragma warning(disable:26110)
	this->shareMutex.unlock();
#pragma warning(default:26110)
}
