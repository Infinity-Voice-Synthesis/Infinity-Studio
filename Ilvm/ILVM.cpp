#include "ILVM.h"

std::unique_ptr<ILVM> ILVM::vm = std::make_unique<ILVM>();
juce::String ILVM::outStrTemp;

std::function<void(juce::String)> ILVM::errorMessage;
std::function<void(juce::String)> ILVM::normalMessage;
std::function<void(void)> ILVM::clearMessage;

std::function<void(void)> ILVM::mainStart;
std::function<void(void)> ILVM::mainStop;

void ILVM::lStdOut(lua_State* L, const char* data, size_t size)
{
	juce::String jstr = juce::String::createStringFromData(data, size);
	ILVM::outStrTemp.append(jstr, jstr.length());
}

void ILVM::lStdOutLine(lua_State* L)
{
	ILVM::vm->normalMessage(ILVM::outStrTemp);
	ILVM::outStrTemp.clear();
}

void ILVM::lStdOutErr(lua_State* L, const char* format, const char* data)
{
	std::string str;
	str.reserve(strlen(format) + strlen(data));
	std::sprintf(str.data(), format, data);
	ILVM::vm->errorMessage(juce::String::createStringFromData(str.c_str(), str.size()));
}

ILVM::ILVM()
{
	LThread::set_destory(this->destoryId);

	set_LUA_InfOChar(ILVM::lStdOut);
	set_LUA_InfOLine(ILVM::lStdOutLine);
	set_LUA_InfOError(ILVM::lStdOutErr);

	ILLibs::reg_mesFunctions(
		[this](juce::String& message) {this->normalMessage(message); },
		[this](juce::String& message) {this->errorMessage(message); },
		[this] {this->clearMessage(); }
	);
	ILLibs::reg_thrFunctions(
		[this](juce::String& id) {return this->findThread(id); },
		[this] {return this->getThreadList(); },
		[this](juce::String& id) {return this->createThread(id); },
		[this](juce::String& id) {return this->removeThread(id); },
		[this](juce::String& id) {return this->threadIsRunning(id); },
		[this](juce::String& id) {return this->destoryThread(id); },
		[this](juce::String& id, juce::String& str) {return this->doStringOnThread(id, str); },
		[this](juce::String& id, juce::String& str) {return this->doFileOnThread(id, str); },
		[this] {this->flushBin(); }
	);
	ILLibs::reg_shrFunctions(
		[this](juce::String& id, juce::String& key, size_t size) {return this->newShare(id, key, size); },
		[this](juce::String& id, juce::String& key) {return this->checkShare(id, key); },
		[this](juce::String& id, juce::String& key) {return this->removeShare(id, key); },
		[this](juce::String& id, juce::String& key) {return this->sizeShare(id, key); },
		[this](juce::String& id, juce::String& key) {return this->getShare(id, key); },
		[this](juce::String& id) {return this->clearShare(id); },
		[this](juce::String& id) {return this->listShare(id); },
		[this](juce::String& id) {this->lockShare(id); },
		[this](juce::String& id) {this->unlockShare(id); }
	);

	DMH::reg_mesFunctions(
		[this](juce::String& message) {this->normalMessage(message); },
		[this](juce::String& message) {this->errorMessage(message); },
		[this] {this->clearMessage(); }
	);
}

ILVM::~ILVM()
{
	this->mainThread = nullptr;

	for (auto thread : this->threads) {
		if (thread->isThreadRunning()) {
			thread->stopThread(0);
			thread->waitForThreadToExit(-1);
		}
		delete thread;
	}
	this->threads.clear();

	for (auto thread : this->threads_bin) {
		if (thread->isThreadRunning()) {
			thread->stopThread(0);
			thread->waitForThreadToExit(-1);
		}
		delete thread;
	}
	this->threads_bin.clear();
}

void ILVM::on_commandsIn(juce::String command)
{
	if (this->mainThread == nullptr) {
		this->mainThread = new LThread;
		this->threads.push_back(this->mainThread);

		if (this->isSafeMode) {
			this->VMPushOptionalFunctions(this->mainThread);
		}
		else {
			this->VMPushAllFunctions(this->mainThread);
		}

		this->mainThread->setId(this->mainId);

		this->normalMessage(juce::String(ILVM_COPYRIGHT));
		this->normalMessage(juce::String(LUA_COPYRIGHT));
	}
	if (!this->mainThread->doString(command)) {
		this->errorMessage("Can't execute the command!");
	}
}

void ILVM::on_threadStart(juce::String id)
{
	if (id == this->mainId) {
		this->mainStart();
	}
}

void ILVM::on_threadStop(juce::String id)
{
	if (id == this->mainId) {
		this->mainStop();
	}
}

void ILVM::mainCritical()
{
	for (auto i = 0; i < this->threads.size(); i++) {
		LThread* thread = this->threads.at(i);
		if (thread->getId() == this->mainId) {
			if (thread->isThreadRunning()) {
				this->mainThread = nullptr;

				this->threads.erase(this->threads.begin() + i);
				this->threads_bin.push_back(thread);

				thread->destory(this->destoryId);

				this->isSafeMode = true;

				this->mainStop();
				this->clearMessage();
				this->errorMessage("Warning!!!!! The blocked Lua thread has been put into the background. Please save the data immediately and restart the editor!");
			}
			break;
		}
	}
}

void ILVM::VMPushOptionalFunctions(LThread* thread)
{
	this->VMPushAllFunctions(thread);//Debug Mode.
}

void ILVM::VMPushAllFunctions(LThread* thread)
{
	thread->beginGlobalTable();

	thread->beginTable("Runtime");
	thread->addFunction("scriptPath", ILLibs::infinity_runtime_scriptPath);
	thread->addFunction("scriptDir", ILLibs::infinity_runtime_scriptDir);
	thread->addFunction("appPath", ILLibs::infinity_runtime_appPath);
	thread->addFunction("appDir", ILLibs::infinity_runtime_appDir);
	thread->addFunction("msleep", ILLibs::infinity_runtime_msleep);
	thread->endTable();//Runtime

	thread->beginTable("Console");
	thread->addFunction("println", ILLibs::infinity_console_println);
	thread->addFunction("error", ILLibs::infinity_console_error);
	thread->addFunction("cls", ILLibs::infinity_console_cls);
	thread->endTable();//Console

	thread->beginTable("Thread");

	thread->beginTable("Share");
	thread->addFunction("create", ILLibs::infinity_thread_share_create);
	thread->addFunction("find", ILLibs::infinity_thread_share_find);
	thread->addFunction("remove", ILLibs::infinity_thread_share_remove);
	thread->addFunction("size", ILLibs::infinity_thread_share_size);
	thread->addFunction("get", ILLibs::infinity_thread_share_get);
	thread->addFunction("clear", ILLibs::infinity_thread_share_clear);
	thread->addFunction("list", ILLibs::infinity_thread_share_list);
	thread->addFunction("lock", ILLibs::infinity_thread_share_lock);
	thread->addFunction("unlock", ILLibs::infinity_thread_share_unlock);
	thread->endTable();

	thread->addFunction("current", ILLibs::infinity_thread_current);
	thread->addFunction("find", ILLibs::infinity_thread_find);
	thread->addFunction("list", ILLibs::infinity_thread_list);
	thread->addFunction("create", ILLibs::infinity_thread_create);
	thread->addFunction("remove", ILLibs::infinity_thread_remove);
	thread->addFunction("check", ILLibs::infinity_thread_check);
	thread->addFunction("destory", ILLibs::infinity_thread_destory);
	thread->addFunction("run", ILLibs::infinity_thread_run);
	thread->addFunction("exec", ILLibs::infinity_thread_exec);
	thread->addFunction("flush", ILLibs::infinity_thread_flush);
	thread->endTable();//Thread

	thread->beginTable("Synth");
	thread->addFunction("test", DMH::test);
	thread->endTable();//Synth

	thread->endGlobalTable("Infinity");

	thread->loadUtils();
}

bool ILVM::findThread(juce::String id)
{
	for (auto t : this->threads) {
		if (t->getId() == id) {
			return true;
		}
	}
	return false;
}

juce::StringArray ILVM::getThreadList()
{
	juce::StringArray list;
	for (auto t : this->threads) {
		list.add(t->getId());
	}
	return list;
}

bool ILVM::createThread(juce::String id)
{
	if (id == this->mainId || id == this->destoryId) {
		return false;
	}
	if (this->findThread(id)) {
		return false;
	}
	for (auto t : this->threads_bin) {
		if (t->getId() == id) {
			return false;
		}
	}

	LThread* thread = new LThread;

	this->threads.push_back(thread);

	this->VMPushAllFunctions(thread);
	thread->setId(id);

	this->normalMessage(juce::String(ILVM_COPYRIGHT));
	this->normalMessage(juce::String(LUA_COPYRIGHT));

	return true;
}

bool ILVM::removeThread(juce::String id)
{
	if (id == this->mainId || id == this->destoryId) {
		return false;
	}
	for (int i = 0; i < this->threads.size(); i++) {
		LThread* t = this->threads.at(i);
		if (t->getId() == id) {
			if (t->isThreadRunning()) {
				return false;
			}

			delete t;
			this->threads.erase(this->threads.begin() + i);

			return true;
		}
	}
	return false;
}

bool ILVM::doStringOnThread(juce::String id, juce::String str)
{
	if (id == this->mainId || id == this->destoryId) {
		return false;
	}
	for (auto t : this->threads) {
		if (t->getId() == id) {
			return t->doString(str);
		}
	}
	return false;
}

bool ILVM::doFileOnThread(juce::String id, juce::String file)
{
	if (id == this->mainId || id == this->destoryId) {
		return false;
	}
	for (auto t : this->threads) {
		if (t->getId() == id) {
			return t->doFile(file);
		}
	}
	return false;
}

bool ILVM::threadIsRunning(juce::String id)
{
	for (auto t : this->threads) {
		if (t->getId() == id) {
			return t->isThreadRunning();
		}
	}
	return false;
}

bool ILVM::destoryThread(juce::String id)
{
	if (id == this->mainId || id == this->destoryId) {
		return false;
	}
	for (int i = 0; i < this->threads.size(); i++) {
		LThread* t = this->threads.at(i);
		if (t->getId() == id) {
			if (t->isThreadRunning()) {
				this->threads.erase(this->threads.begin() + i);
				this->threads_bin.push_back(t);

				t->destory(this->destoryId);

				return true;
			}
		}
	}
	return false;
}

void ILVM::flushBin()
{
	for (int i = this->threads_bin.size() - 1; i >= 0; i--) {
		LThread* t = this->threads_bin.at(i);
		if (!t->isThreadRunning()) {
			delete t;
			this->threads_bin.erase(this->threads_bin.begin() + i);
		}
	}
}

bool ILVM::checkShare(juce::String id, juce::String key)
{
	if (id == this->destoryId) {
		return false;
	}
	for (auto t : this->threads) {
		if (t->getId() == id) {
			return t->checkShare(key);
		}
	}
	return false;
}

void* ILVM::newShare(juce::String id, juce::String key, size_t size)
{
	if (id == this->destoryId) {
		return nullptr;
	}
	for (auto t : this->threads) {
		if (t->getId() == id) {
			return t->newShare(key, size);
		}
	}
	return nullptr;
}

bool ILVM::removeShare(juce::String id, juce::String key)
{
	if (id == this->destoryId) {
		return false;
	}
	for (auto t : this->threads) {
		if (t->getId() == id) {
			return t->removeShare(key);
		}
	}
	return false;
}

void* ILVM::getShare(juce::String id, juce::String key)
{
	if (id == this->destoryId) {
		return nullptr;
	}
	for (auto t : this->threads) {
		if (t->getId() == id) {
			return t->getShare(key);
		}
	}
	return nullptr;
}

size_t ILVM::sizeShare(juce::String id, juce::String key)
{
	if (id == this->destoryId) {
		return 0;
	}
	for (auto t : this->threads) {
		if (t->getId() == id) {
			return t->sizeShare(key);
		}
	}
	return 0;
}

bool ILVM::clearShare(juce::String id)
{
	if (id == this->destoryId) {
		return false;
	}
	for (auto t : this->threads) {
		if (t->getId() == id) {
			return t->clearShare();
		}
	}
	return false;
}

juce::StringArray ILVM::listShare(juce::String id)
{
	if (id == this->destoryId) {
		return juce::StringArray();
	}
	for (auto t : this->threads) {
		if (t->getId() == id) {
			return t->listShare();
		}
	}
	return juce::StringArray();
}

void ILVM::lockShare(juce::String id)
{
	if (id == this->destoryId) {
		return;
	}
	for (auto t : this->threads) {
		if (t->getId() == id) {
			return t->lockShare();
		}
	}
	return;
}

void ILVM::unlockShare(juce::String id)
{
	if (id == this->destoryId) {
		return;
	}
	for (auto t : this->threads) {
		if (t->getId() == id) {
			return t->unlockShare();
		}
	}
	return;
}
