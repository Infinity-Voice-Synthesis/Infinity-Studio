#include "LThread.h"

extern "C" {
#include "Lua/lstate.h"
}

QString LThread::destoryId;

void LThread::set_destory(QString destoryId)
{
	LThread::destoryId = destoryId;
}

void LThread::hookFunction(lua_State* L, lua_Debug* ar)
{
	QMutex* mutex = (QMutex*)L->thread_mutex;
	QString* str = (QString*)L->thread_id;
	mutex->lock();
	QString tName = (*str);
	mutex->unlock();
	if (tName == LThread::destoryId) {
		lua_pushstring(L, "This thread is destoried!");
		lua_error(L);
	}
}

LThread::LThread(QObject *parent)
	: QThread(parent)
{
	this->lstate = luaL_newstate();
	this->lstate->thread_id = (void*)(&(this->Id));
	this->lstate->thread_mutex = (void*)(&(this->idMutex));

	lua_sethook(this->lstate, LThread::hookFunction, LUA_MASKLINE, 0);

	luaL_openlibs(this->lstate);

	bool error1 = luaL_dostring(this->lstate, QString("package.path = '" + QCoreApplication::applicationDirPath() + "/scripts/?.lua;;"+ QCoreApplication::applicationDirPath() + "/scripts/?/?.lua;'").toStdString().c_str());
	if (error1) {
		qDebug("Lua Error:%s", qPrintable(lua_tostring(this->lstate, -1)));
		lua_pop(this->lstate, 1);
	}
	bool error2 = luaL_dostring(this->lstate, QString("package.cpath = '" + QCoreApplication::applicationDirPath() + "/scripts/libs/?.dll;'").toStdString().c_str());
	if (error2) {
		qDebug("Lua Error:%s", qPrintable(lua_tostring(this->lstate, -1)));
		lua_pop(this->lstate, 1);
	}
}

LThread::~LThread()
{
	lua_close(this->lstate);

	for (auto& p : this->shareData) {
		free(p.second);
	}
	this->shareData.clear();
}

void LThread::run()
{
	emit this->tStarted(this->Id);
	if (this->tType == LType::DoFile) {
		bool error = luaL_dofile(this->lstate, this->lFileName.toStdString().c_str());
		if (error) {
			emit this->errorMessage(QString::fromStdString(lua_tostring(this->lstate, -1)));
			lua_pop(this->lstate, 1);
			emit this->tEnded(this->Id);
			return;
		}
	}
	while (this->strList.size() > 0) {
		bool error = luaL_dostring(this->lstate, this->strList.dequeue().toStdString().c_str());
		if (error) {
			emit this->errorMessage(QString::fromStdString(lua_tostring(this->lstate, -1)));
			lua_pop(this->lstate, 1);
		}
	}
	emit this->tEnded(this->Id);
	quit();
}

bool LThread::doFile(QString name)
{
	if (!this->Id.isEmpty()) {
		if (!this->isRunning()) {
			this->tType = LType::DoFile;
			this->lFileName = name;
			this->start();
			return true;
		}
	}
	return false;
}

bool LThread::doString(QString str)
{
	if (!this->Id.isEmpty()) {
		this->tType = LType::DoString;
		this->strList.enqueue(str);
		if (!this->isRunning()) {
			this->start();
		}
		else {
			emit this->normalMessage("The VM is running.Command will wait in queue.");
		}
		return true;
	}
	return false;
}

bool LThread::setId(QString id)
{
	if (!id.isEmpty()) {
		if (this->Id.isEmpty()) {
			this->Id = id;
			return true;
		}
	}
	return false;
}

QString LThread::getId()
{
	return this->Id;
}

void LThread::beginGlobalTable()
{
	lua_newtable(this->lstate);
}

void LThread::endGlobalTable(QString name)
{
	lua_setglobal(this->lstate, name.toStdString().c_str());
}

void LThread::beginTable(QString name)
{
	lua_pushstring(this->lstate, name.toStdString().c_str());
	lua_newtable(this->lstate);
}

void LThread::endTable()
{
	lua_settable(this->lstate, -3);
}

void LThread::addFunction(QString name, lua_CFunction function)
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
		emit this->errorMessage(QString::fromStdString(lua_tostring(this->lstate, -1)));
		lua_pop(this->lstate, 1);
	}
	error = luaL_dostring(this->lstate, "Infinity.Console.print = print");
	if (error) {
		emit this->errorMessage(QString::fromStdString(lua_tostring(this->lstate, -1)));
		lua_pop(this->lstate, 1);
	}
}

bool LThread::destory(QString id)
{
	if (this->isRunning()) {
		this->idMutex.lock();
		this->Id = id;
		this->idMutex.unlock();
		return true;
	}
	return false;
}

bool LThread::checkShare(QString key)
{
	this->shareMutex.lock();
	bool result = this->shareData.contains(key);
	this->shareMutex.unlock();
	return result;
}

void* LThread::newShare(QString key, size_t size)
{
	if (checkShare(key)) {
		return nullptr;
	}
	this->shareMutex.lock();
	void* result = malloc(size);
	this->shareData.insert(key, qMakePair(size, result));
	this->shareMutex.unlock();
	return result;
}

bool LThread::removeShare(QString key)
{
	if (!checkShare(key)) {
		return false;
	}
	this->shareMutex.lock();
	free(this->shareData.value(key).second);
	this->shareData.remove(key);
	this->shareMutex.unlock();
	return true;
}

void* LThread::getShare(QString key)
{
	if (!checkShare(key)) {
		return nullptr;
	}
	this->shareMutex.lock();
	void* result = this->shareData.value(key).second;
	this->shareMutex.unlock();
	return result;
}

size_t LThread::sizeShare(QString key)
{
	if (!checkShare(key)) {
		return 0;
	}
	this->shareMutex.lock();
	size_t result = this->shareData.value(key).first;
	this->shareMutex.unlock();
	return result;
}

bool LThread::clearShare()
{
	this->shareMutex.lock();
	for (auto& p : this->shareData) {
		if (p.second != nullptr) {
			free(p.second);
		}
	}
	this->shareData.clear();
	this->shareMutex.unlock();
	return true;
}

QStringList LThread::listShare()
{
	this->shareMutex.lock();
	QStringList result = this->shareData.keys();
	this->shareMutex.unlock();
	return result;
}

void LThread::lockShare()
{
	this->shareMutex.lock();
}

void LThread::unlockShare()
{
	this->shareMutex.unlock();
}
