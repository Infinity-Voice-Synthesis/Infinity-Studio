#pragma once

#include <QObject>
#include <QList>
#include "LThread.h"
#include "Infinity_global.h"
#include "Infinity_Events.h"
#include <QMessageBox>
#include "ILLibs.h"
#include "DMH.h"

#define ILVM_COPYRIGHT "Infinity Studio Script Interpreter 1.0.0" "  Copyright (C) 2019-2022 Infinity Synthesis Team"

class ILVM : public QObject
{
	Q_OBJECT
	
	ILVM(QObject *parent);
	~ILVM();

	static ILVM vm;

	QList<LThread*> threads;
	QList<LThread*> threads_bin;
	LThread* mainThread = nullptr;

	const QString mainId = "_main";
	const QString destoryId = "_destory";

	static QString outStrTemp;

	bool isSafeMode = false;

	void VMPushOptionalFunctions(LThread* thread);
	void VMPushAllFunctions(LThread* thread);

public:
	static ILVM& getVM();

	static void lStdOut(lua_State* L, const char* data, size_t size);
	static void lStdOutLine(lua_State* L);
	static void lStdOutErr(lua_State* L, const char* format, const char* data);

	void mainCritical();

	bool findThread(QString id);
	QStringList getThreadList();
	bool createThread(QString id);
	bool removeThread(QString id);

	bool destoryThread(QString id);

	bool doStringOnThread(QString id, QString str);
	bool doFileOnThread(QString id, QString file);

	bool threadIsRunning(QString id);

	void flushBin();

	bool checkShare(QString id, QString key);
	void* newShare(QString id, QString key, size_t size);
	bool removeShare(QString id, QString key);
	void* getShare(QString id, QString key);
	size_t sizeShare(QString id, QString key);
	bool clearShare(QString id);
	QStringList listShare(QString id);
	void lockShare(QString id);
	void unlockShare(QString id);

public slots:
	void on_commandsIn(QString command);

private slots:
	void on_threadStart(QString id);
	void on_threadStop(QString id);

signals:
	void errorMessage(QString message);
	void normalMessage(QString message);
	void clearMessage();

	void mainStart();
	void mainStop();
};
