#include "ILVM.h"

ILVM ILVM::vm = ILVM(nullptr);
QString ILVM::outStrTemp;

void ILVM::lStdOut(lua_State* L, const char* data, size_t size)
{
	for (int i = 0; i < size; i++) {
		ILVM::outStrTemp.append(data[i]);
		QApplication::processEvents();
	}
}

void ILVM::lStdOutLine(lua_State* L)
{
	emit ILVM::getVM().normalMessage(ILVM::outStrTemp);
	ILVM::outStrTemp.clear();
}

void ILVM::lStdOutErr(lua_State* L, const char* format, const char* data)
{
	emit ILVM::getVM().errorMessage(QString::asprintf(format, data));
}

ILVM::ILVM(QObject *parent)
	: QObject(parent)
{
	LThread::set_destory(this->destoryId);

	set_LUA_InfOChar(ILVM::lStdOut);
	set_LUA_InfOLine(ILVM::lStdOutLine);
	set_LUA_InfOError(ILVM::lStdOutErr);

	ILLibs::reg_mesFunctions(
		[this](QString& message) {emit this->normalMessage(message); },
		[this](QString& message) {emit this->errorMessage(message); },
		[this] {emit this->clearMessage(); }
	);
	ILLibs::reg_thrFunctions(
		[this](QString& id) {return this->findThread(id); },
		[this] {return this->getThreadList(); },
		[this](QString& id) {return this->createThread(id); },
		[this](QString& id) {return this->removeThread(id); },
		[this](QString& id) {return this->threadIsRunning(id); },
		[this](QString& id) {return this->destoryThread(id); },
		[this](QString& id, QString& str) {return this->doStringOnThread(id, str); },
		[this](QString& id, QString& str) {return this->doFileOnThread(id, str); },
		[this] {this->flushBin(); }
	);
	ILLibs::reg_shrFunctions(
		[this](QString& id, QString& key, size_t size) {return this->newShare(id, key, size); },
		[this](QString& id, QString& key) {return this->checkShare(id, key); },
		[this](QString& id, QString& key) {return this->removeShare(id, key); },
		[this](QString& id, QString& key) {return this->sizeShare(id, key); },
		[this](QString& id, QString& key) {return this->getShare(id, key); },
		[this](QString& id) {return this->clearShare(id); },
		[this](QString& id) {return this->listShare(id); },
		[this](QString& id) {this->lockShare(id); },
		[this](QString& id) {this->unlockShare(id); }
	);

	DMH::reg_mesFunctions(
		[this](QString& message) {emit this->normalMessage(message); },
		[this](QString& message) {emit this->errorMessage(message); },
		[this] {emit this->clearMessage(); }
	);
}

ILVM::~ILVM()
{
	this->mainThread = nullptr;

	for (auto thread : this->threads) {
		if (thread->isRunning()) {
			thread->terminate();
			thread->wait();
		}
		thread->deleteLater();
	}
	this->threads.clear();

	for (auto thread : this->threads_bin) {
		if (thread->isRunning()) {
			thread->terminate();
			thread->wait();
		}
		thread->deleteLater();
	}
	this->threads_bin.clear();
}

ILVM& ILVM::getVM()
{
	return ILVM::vm;
}

void ILVM::on_commandsIn(QString command)
{
	if (this->mainThread == nullptr) {
		while (this->mainThread == nullptr) {
			this->mainThread = new(std::nothrow) LThread(this);
			if (this->mainThread == nullptr) {
				QMessageBox::Button result = QMessageBox::critical(nullptr, "Infinity Studio 0", "Application can't alloc memory for object \"mainThread\" on heap!\nPlease check your memory then retry or abort this application!", QMessageBox::Retry | QMessageBox::Button::Abort, QMessageBox::Abort);
				if (result != QMessageBox::Retry) {
					return ;
				}
			}
		}
		this->threads.append(this->mainThread);

		if (this->isSafeMode) {
			this->VMPushOptionalFunctions(this->mainThread);
		}
		else {
			this->VMPushAllFunctions(this->mainThread);
		}

		connect(this->mainThread, &LThread::errorMessage, this, &ILVM::errorMessage, Qt::ConnectionType::QueuedConnection);
		connect(this->mainThread, &LThread::normalMessage, this, &ILVM::normalMessage, Qt::ConnectionType::QueuedConnection);
		connect(this->mainThread, &LThread::tStarted, this, &ILVM::on_threadStart, Qt::ConnectionType::QueuedConnection);
		connect(this->mainThread, &LThread::tEnded, this, &ILVM::on_threadStop, Qt::ConnectionType::QueuedConnection);

		this->mainThread->setId(this->mainId);

		emit this->normalMessage(QString(ILVM_COPYRIGHT));
		emit this->normalMessage(QString(LUA_COPYRIGHT));
	}
	if (!this->mainThread->doString(command)) {
		emit this->errorMessage("Can't execute the command!");
	}
}

void ILVM::on_threadStart(QString id)
{
	if (id == this->mainId) {
		emit this->mainStart();
	}
}

void ILVM::on_threadStop(QString id)
{
	if (id == this->mainId) {
		emit this->mainStop();
	}
}

void ILVM::mainCritical()
{
	for (int i = 0; i < this->threads.size(); i++) {
		LThread* thread = this->threads.at(i);
		if (thread->getId() == this->mainId) {
			if (thread->isRunning()) {
				this->mainThread = nullptr;

				disconnect(thread, &LThread::errorMessage, this, &ILVM::errorMessage);
				disconnect(thread, &LThread::normalMessage, this, &ILVM::normalMessage);
				disconnect(thread, &LThread::tStarted, this, &ILVM::on_threadStart);
				disconnect(thread, &LThread::tEnded, this, &ILVM::on_threadStop);

				this->threads.removeAt(i);
				this->threads_bin.append(thread);

				thread->destory(this->destoryId);
				thread->quit();

				this->isSafeMode = true;

				emit this->mainStop();
				emit this->clearMessage();
				emit this->errorMessage("Warning!!!!! The blocked Lua thread has been put into the background. Please save the data immediately and restart the editor!");
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

bool ILVM::findThread(QString id)
{
	for (auto t : this->threads) {
		if (t->getId() == id) {
			return true;
		}
	}
	return false;
}

QStringList ILVM::getThreadList()
{
	QStringList list;
	for (auto t : this->threads) {
		list.append(t->getId());
	}
	return list;
}

bool ILVM::createThread(QString id)
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

	LThread* thread = nullptr;

	while (thread == nullptr) {
		thread = new(std::nothrow) LThread(this);
		if (thread == nullptr) {
			QMessageBox::Button result = QMessageBox::critical(nullptr, "Infinity Studio 0", "Application can't alloc memory for object \"thread\" on heap!\nPlease check your memory then retry or abort this application!", QMessageBox::Retry | QMessageBox::Button::Abort, QMessageBox::Abort);
			if (result != QMessageBox::Retry) {
				return false;
			}
		}
	}

	this->threads.append(thread);

	this->VMPushAllFunctions(thread);

	connect(thread, &LThread::errorMessage, this, &ILVM::errorMessage, Qt::ConnectionType::QueuedConnection);
	connect(thread, &LThread::normalMessage, this, &ILVM::normalMessage, Qt::ConnectionType::QueuedConnection);
	
	thread->setId(id);

	emit this->normalMessage(QString(ILVM_COPYRIGHT));
	emit this->normalMessage(QString(LUA_COPYRIGHT));

	return true;
}

bool ILVM::removeThread(QString id)
{
	if (id == this->mainId || id == this->destoryId) {
		return false;
	}
	for (int i = 0; i < this->threads.size(); i++) {
		LThread* t = this->threads.at(i);
		if (t->getId() == id) {
			if (t->isRunning()) {
				return false;
			}
			disconnect(t, &LThread::errorMessage, this, &ILVM::errorMessage);
			disconnect(t, &LThread::normalMessage, this, &ILVM::normalMessage);

			t->deleteLater();

			this->threads.removeAt(i);

			return true;
		}
	}
	return false;
}

bool ILVM::doStringOnThread(QString id, QString str)
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

bool ILVM::doFileOnThread(QString id, QString file)
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

bool ILVM::threadIsRunning(QString id)
{
	for (auto t : this->threads) {
		if (t->getId() == id) {
			return t->isRunning();
		}
	}
	return false;
}

bool ILVM::destoryThread(QString id)
{
	if (id == this->mainId || id == this->destoryId) {
		return false;
	}
	for (int i = 0; i < this->threads.size(); i++) {
		LThread* t = this->threads.at(i);
		if (t->getId() == id) {
			if (t->isRunning()) {
				disconnect(t, &LThread::errorMessage, this, &ILVM::errorMessage);
				disconnect(t, &LThread::normalMessage, this, &ILVM::normalMessage);

				this->threads.removeAt(i);
				this->threads_bin.append(t);

				t->destory(this->destoryId);
				t->quit();

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
		if (!t->isRunning()) {
			t->deleteLater();
			this->threads_bin.removeAt(i);
		}
	}
}

bool ILVM::checkShare(QString id, QString key)
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

void* ILVM::newShare(QString id, QString key, size_t size)
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

bool ILVM::removeShare(QString id, QString key)
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

void* ILVM::getShare(QString id, QString key)
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

size_t ILVM::sizeShare(QString id, QString key)
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

bool ILVM::clearShare(QString id)
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

QStringList ILVM::listShare(QString id)
{
	if (id == this->destoryId) {
		return QStringList();
	}
	for (auto t : this->threads) {
		if (t->getId() == id) {
			return t->listShare();
		}
	}
	return QStringList();
}

void ILVM::lockShare(QString id)
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

void ILVM::unlockShare(QString id)
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
