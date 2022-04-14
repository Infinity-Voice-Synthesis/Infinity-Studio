#include "ILVM.h"
#include "llibs/ILLibs.h"
#include "llibs/DMH.h"

//In this file,there were many ass warnings C6031,then I disabled the warning C6031 near the places
//which cause that warning..

std::unique_ptr<ILVM> ILVM::vm = std::make_unique<ILVM>();
juce::String ILVM::outStrTemp;

std::function<void(juce::StringRef)> ILVM::errorMessage;
std::function<void(juce::StringRef)> ILVM::normalMessage;
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
	std::function<void(juce::StringRef)> errorMessage,
	std::function<void(juce::StringRef)> normalMessage,
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
	set_LUA_InfOChar(ILVM::lStdOut);
	set_LUA_InfOLine(ILVM::lStdOutLine);
	set_LUA_InfOError(ILVM::lStdOutErr);

	ILLibs::reg_mesFunctions(
		[](juce::StringRef message) {ILVM::vm->normalMessage(message); },
		[](juce::StringRef message) {ILVM::vm->errorMessage(message); },
		[] {ILVM::vm->clearMessage(); }
	);
	ILLibs::reg_thrFunctions(
		[](juce::StringRef id) {return ILVM::vm->findThread(id); },
		[] {return ILVM::vm->getThreadList(); },
		[](juce::StringRef id) {return ILVM::vm->createThread(id); },
		[](juce::StringRef id) {return ILVM::vm->removeThread(id); },
		[](juce::StringRef id) {return ILVM::vm->threadIsRunning(id); },
		[](juce::StringRef id) {return ILVM::vm->destoryThread(id); },
		[](juce::StringRef id, juce::StringRef str) {return ILVM::vm->doStringOnThread(id, str); },
		[](juce::StringRef id, juce::StringRef str) {return ILVM::vm->doFileOnThread(id, str); },
		[] {ILVM::vm->flushBin(); }
	);
	ILLibs::reg_shrFunctions(
		[](juce::StringRef id, juce::StringRef key, size_t size) {return ILVM::vm->newShare(id, key, size); },
		[](juce::StringRef id, juce::StringRef key) {return ILVM::vm->checkShare(id, key); },
		[](juce::StringRef id, juce::StringRef key) {return ILVM::vm->removeShare(id, key); },
		[](juce::StringRef id, juce::StringRef key) {return ILVM::vm->sizeShare(id, key); },
		[](juce::StringRef id, juce::StringRef key) {return ILVM::vm->getShare(id, key); },
		[](juce::StringRef id) {return ILVM::vm->clearShare(id); },
		[](juce::StringRef id) {return ILVM::vm->listShare(id); },
		[](juce::StringRef id) {ILVM::vm->lockShare(id); },
		[](juce::StringRef id) {ILVM::vm->unlockShare(id); }
	);

	DMH::reg_mesFunctions(
		[](juce::StringRef message) {ILVM::vm->normalMessage(message); },
		[](juce::StringRef message) {ILVM::vm->errorMessage(message); },
		[] {ILVM::vm->clearMessage(); }
	);
#pragma warning(default:6031)
}

void ILVM::destory()
{
	ILVM::vm = nullptr;
}

void ILVM::run(juce::StringRef code)
{
	ILVM::vm->commandsIn(code);
}

void ILVM::stop()
{
	ILVM::vm->mainCritical();
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
			thread->destory(this->destoryId);
			//thread->stopThread(0);
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

void ILVM::commandsIn(juce::StringRef command)
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
			[](juce::StringRef str) {ILVM::errorMessage(str); },
			[](juce::StringRef str) {ILVM::normalMessage(str); },
			[](juce::StringRef id) {ILVM::on_threadStart(id); },
			[](juce::StringRef id) {ILVM::on_threadStop(id); }
		);
	}
	if (!this->mainThread->doString(command)) {
		this->errorMessage("Can't execute the command!");
	}
}

void ILVM::on_threadStart(juce::StringRef id)
{
	if (id == ILVM::vm->mainId) {
#pragma warning(disable:6031)
		ILVM::vm->mainStart();
#pragma warning(default:6031)
	}
}

void ILVM::on_threadStop(juce::StringRef id)
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

bool ILVM::findThread(juce::StringRef id)
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

bool ILVM::createThread(juce::StringRef id)
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
		[](juce::StringRef str) {ILVM::errorMessage(str); },
		[](juce::StringRef str) {ILVM::normalMessage(str); },
		[](juce::StringRef id) {ILVM::on_threadStart(id); },
		[](juce::StringRef id) {ILVM::on_threadStop(id); }
	);

	return true;
}

bool ILVM::removeThread(juce::StringRef id)
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

bool ILVM::doStringOnThread(juce::StringRef id, juce::StringRef str)
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

bool ILVM::doFileOnThread(juce::StringRef id, juce::StringRef file)
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

bool ILVM::threadIsRunning(juce::StringRef id)
{
	for (auto t : this->threads) {
		if (t->getId() == id) {
			return t->isThreadRunning();
		}
	}
	return false;
}

bool ILVM::destoryThread(juce::StringRef id)
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

bool ILVM::checkShare(juce::StringRef id, juce::StringRef key)
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

void* ILVM::newShare(juce::StringRef id, juce::StringRef key, size_t size)
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

bool ILVM::removeShare(juce::StringRef id, juce::StringRef key)
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

void* ILVM::getShare(juce::StringRef id, juce::StringRef key)
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

size_t ILVM::sizeShare(juce::StringRef id, juce::StringRef key)
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

bool ILVM::clearShare(juce::StringRef id)
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

juce::StringArray ILVM::listShare(juce::StringRef id)
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

void ILVM::lockShare(juce::StringRef id)
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

void ILVM::unlockShare(juce::StringRef id)
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
