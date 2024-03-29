﻿#include "LThread.h"
#include "utils/Config.h"
#include "ILVM.h"

extern "C" {
#include "Lua/lstate.h"
}

void LThread::connect(
	std::function<void(juce::StringRef)> errorMessage,
	std::function<void(juce::StringRef)> normalMessage,
	std::function<void(juce::StringRef)> tStarted,
	std::function<void(juce::StringRef)> tEnded
)
{
	this->errorMessage = errorMessage;
	this->normalMessage = normalMessage;
	this->tStarted = tStarted;
	this->tEnded = tEnded;
}

void LThread::disconnect()
{
	this->errorMessage = [](juce::StringRef){};
	this->normalMessage = [](juce::StringRef) {};
	this->tStarted = [](juce::StringRef) {};
	this->tEnded = [](juce::StringRef) {};
}

void LThread::hookFunction(lua_State* L, lua_Debug* ar)
{
	juce::ReadWriteLock* mutex = (juce::ReadWriteLock*)L->thread_mutex;
	juce::String* str = (juce::String*)L->thread_id;
	mutex->enterRead();
	juce::String tName = (*str);
	mutex->exitRead();
	if (!ILVM::vm || tName == ILVM::vm->destoryId) {
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
			this->idMutex.enterRead();
			juce::String tName = this->Id;
			this->idMutex.exitRead();
			if (!ILVM::vm || tName == ILVM::vm->destoryId) {
				return;
			}
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
			this->idMutex.enterRead();
			juce::String tName = this->Id;
			this->idMutex.exitRead();
			if (!ILVM::vm || tName == ILVM::vm->destoryId) {
				return;
			}
			this->errorMessage(juce::String::createStringFromData(str.c_str(), str.size()));
			lua_pop(this->lstate, 1);
		}
	}
	this->tEnded(this->Id);
}

bool LThread::doFile(juce::StringRef name)
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

bool LThread::doString(juce::StringRef str)
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

bool LThread::setId(juce::StringRef id)
{
	if (!id.isEmpty()) {
		if (this->Id.isEmpty()) {
			this->Id = id;
			return true;
		}
	}
	return false;
}

juce::StringRef LThread::getId()
{
	return this->Id;
}

void LThread::beginGlobalTable()
{
	lua_newtable(this->lstate);
}

void LThread::endGlobalTable(juce::StringRef name)
{
	lua_setglobal(this->lstate, name);	
}

void LThread::beginTable(juce::StringRef name)
{
	lua_pushstring(this->lstate, name);
	lua_newtable(this->lstate);
}

void LThread::endTable()
{
	lua_settable(this->lstate, -3);
}

void LThread::addFunction(juce::StringRef name, lua_CFunction function)
{
	lua_pushstring(this->lstate, name);
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

bool LThread::destory(juce::StringRef id)
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

bool LThread::checkShare(juce::StringRef key)
{
	this->shareMutex.lock();
	bool result = this->shareData.contains(key);
	this->shareMutex.unlock();
	return result;
}

void* LThread::newShare(juce::StringRef key, size_t size)
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

bool LThread::removeShare(juce::StringRef key)
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

void* LThread::getShare(juce::StringRef key)
{
	if (!checkShare(key)) {
		return nullptr;
	}
	this->shareMutex.lock();
	void* result = this->shareData.getReference(key).second;
	this->shareMutex.unlock();
	return result;
}

size_t LThread::sizeShare(juce::StringRef key)
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
