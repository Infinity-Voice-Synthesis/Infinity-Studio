#include "ILVM.h"
#include "llibs/ILLibs.h"
#include "llibs/DMH.h"

//In this file,there were many ass warnings C6031,then I disabled the warning C6031 near the places
//which cause that warning..

std::unique_ptr<ILVM> ILVM::vm = std::make_unique<ILVM>();
juce::String ILVM::outStrTemp;

std::function<void(const juce::String&)> ILVM::errorMessage;
std::function<void(const juce::String&)> ILVM::normalMessage;
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
#pragma warning(disable:6031)
	ILVM::vm->normalMessage(ILVM::outStrTemp);
#pragma warning(default:6031)
	ILVM::outStrTemp.clear();
}

void ILVM::lStdOutErr(lua_State* L, const char* format, const char* data)
{
	std::string str;
	str.reserve(strlen(format) + strlen(data));
	std::sprintf(str.data(), format, data);
#pragma warning(disable:6031)
	ILVM::vm->errorMessage(juce::String::createStringFromData(str.c_str(), str.size()));
#pragma warning(default:6031)
}

void ILVM::init(
	std::function<void(const juce::String&)> errorMessage,
	std::function<void(const juce::String&)> normalMessage,
	std::function<void(void)> clearMessage,
	std::function<void(void)> mainStart,
	std::function<void(void)> mainStop
)
{
	ILVM::errorMessage = errorMessage;
	ILVM::normalMessage = normalMessage;
	ILVM::clearMessage = clearMessage;
	ILVM::mainStart = mainStart;
	ILVM::mainStop = mainStop;

#pragma warning(disable:6031)
	LThread::set_destory(ILVM::vm->destoryId);

	set_LUA_InfOChar(ILVM::lStdOut);
	set_LUA_InfOLine(ILVM::lStdOutLine);
	set_LUA_InfOError(ILVM::lStdOutErr);

	ILLibs::reg_mesFunctions(
		[](const juce::String& message) {ILVM::vm->normalMessage(message); },
		[](const juce::String& message) {ILVM::vm->errorMessage(message); },
		[] {ILVM::vm->clearMessage(); }
	);
	ILLibs::reg_thrFunctions(
		[](const juce::String& id) {return ILVM::vm->findThread(id); },
		[] {return ILVM::vm->getThreadList(); },
		[](const juce::String& id) {return ILVM::vm->createThread(id); },
		[](const juce::String& id) {return ILVM::vm->removeThread(id); },
		[](const juce::String& id) {return ILVM::vm->threadIsRunning(id); },
		[](const juce::String& id) {return ILVM::vm->destoryThread(id); },
		[](const juce::String& id, const juce::String& str) {return ILVM::vm->doStringOnThread(id, str); },
		[](const juce::String& id, const juce::String& str) {return ILVM::vm->doFileOnThread(id, str); },
		[] {ILVM::vm->flushBin(); }
	);
	ILLibs::reg_shrFunctions(
		[](const juce::String& id, const juce::String& key, size_t size) {return ILVM::vm->newShare(id, key, size); },
		[](const juce::String& id, const juce::String& key) {return ILVM::vm->checkShare(id, key); },
		[](const juce::String& id, const juce::String& key) {return ILVM::vm->removeShare(id, key); },
		[](const juce::String& id, const juce::String& key) {return ILVM::vm->sizeShare(id, key); },
		[](const juce::String& id, const juce::String& key) {return ILVM::vm->getShare(id, key); },
		[](const juce::String& id) {return ILVM::vm->clearShare(id); },
		[](const juce::String& id) {return ILVM::vm->listShare(id); },
		[](const juce::String& id) {ILVM::vm->lockShare(id); },
		[](const juce::String& id) {ILVM::vm->unlockShare(id); }
	);

	DMH::reg_mesFunctions(
		[](const juce::String& message) {ILVM::vm->normalMessage(message); },
		[](const juce::String& message) {ILVM::vm->errorMessage(message); },
		[] {ILVM::vm->clearMessage(); }
	);
#pragma warning(default:6031)
}

void ILVM::destory()
{
	ILVM::vm = nullptr;
}

ILVM::ILVM()
{
	
}

ILVM::~ILVM()
{
	this->mainThread = nullptr;

	for (auto thread : this->threads) {
		thread->disconnect();
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

void ILVM::on_commandsIn(const juce::String& command)
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

		this->normalMessage(ILVM_COPYRIGHT);
		this->normalMessage(LUA_COPYRIGHT);

		this->mainThread->connect(
			[](const juce::String& str) {ILVM::errorMessage(str); },
			[](const juce::String& str) {ILVM::normalMessage(str); },
			[](const juce::String& id) {ILVM::on_threadStart(id); },
			[](const juce::String& id) {ILVM::on_threadStop(id); }
		);
	}
	if (!this->mainThread->doString(command)) {
		this->errorMessage("Can't execute the command!");
	}
}

void ILVM::on_threadStart(const juce::String& id)
{
	if (id == ILVM::vm->mainId) {
#pragma warning(disable:6031)
		ILVM::vm->mainStart();
#pragma warning(default:6031)
	}
}

void ILVM::on_threadStop(const juce::String& id)
{
	if (id == ILVM::vm->mainId) {
#pragma warning(disable:6031)
		ILVM::vm->mainStop();
#pragma warning(default:6031)
	}
}

void ILVM::mainCritical()
{
	for (auto i = 0; i < this->threads.size(); i++) {
		LThread* thread = this->threads.at(i);
		if (thread->getId() == this->mainId) {
			if (thread->isThreadRunning()) {
				thread->disconnect();
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

bool ILVM::findThread(const juce::String& id)
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

bool ILVM::createThread(const juce::String& id)
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

	this->normalMessage(ILVM_COPYRIGHT);
	this->normalMessage(LUA_COPYRIGHT);

	thread->connect(
		[](const juce::String& str) {ILVM::errorMessage(str); },
		[](const juce::String& str) {ILVM::normalMessage(str); },
		[](const juce::String& id) {ILVM::on_threadStart(id); },
		[](const juce::String& id) {ILVM::on_threadStop(id); }
	);

	return true;
}

bool ILVM::removeThread(const juce::String& id)
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

			t->disconnect();

			delete t;
			this->threads.erase(this->threads.begin() + i);

			return true;
		}
	}
	return false;
}

bool ILVM::doStringOnThread(const juce::String& id, const juce::String& str)
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

bool ILVM::doFileOnThread(const juce::String& id, const juce::String& file)
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

bool ILVM::threadIsRunning(const juce::String& id)
{
	for (auto t : this->threads) {
		if (t->getId() == id) {
			return t->isThreadRunning();
		}
	}
	return false;
}

bool ILVM::destoryThread(const juce::String& id)
{
	if (id == this->mainId || id == this->destoryId) {
		return false;
	}
	for (int i = 0; i < this->threads.size(); i++) {
		LThread* t = this->threads.at(i);
		if (t->getId() == id) {
			if (t->isThreadRunning()) {
				t->disconnect();
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

bool ILVM::checkShare(const juce::String& id, const juce::String& key)
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

void* ILVM::newShare(const juce::String& id, const juce::String& key, size_t size)
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

bool ILVM::removeShare(const juce::String& id, const juce::String& key)
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

void* ILVM::getShare(const juce::String& id, const juce::String& key)
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

size_t ILVM::sizeShare(const juce::String& id, const juce::String& key)
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

bool ILVM::clearShare(const juce::String& id)
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

juce::StringArray ILVM::listShare(const juce::String& id)
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

void ILVM::lockShare(const juce::String& id)
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

void ILVM::unlockShare(const juce::String& id)
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
