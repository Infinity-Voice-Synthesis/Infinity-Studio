#pragma once

#include <QThread>
#include <QCoreApplication>
#include <QQueue>
#include <QMutex>
#include <QMap>

#include "Lua/lua.hpp"

#include "ILLibs.h"

class LThread : public QThread
{
	Q_OBJECT

public:
	LThread(QObject *parent);
	~LThread();

	bool doFile(QString name);
	bool doString(QString str);

	bool setId(QString id);
	QString getId();

	bool destory(QString id);

	void beginGlobalTable();
	void endGlobalTable(QString name);

	void beginTable(QString name);
	void endTable();

	void addFunction(QString name, lua_CFunction function);

	void loadUtils();

	static void set_destory(QString destoryId);

	bool checkShare(QString key);
	void* newShare(QString key, size_t size);
	bool removeShare(QString key);
	void* getShare(QString key);
	size_t sizeShare(QString key);
	bool clearShare();
	QStringList listShare();
	void lockShare();
	void unlockShare();

private:
	lua_State* lstate = nullptr;

	QString lFileName;
	QString Id;
	QMutex idMutex;

	enum class LType {
		DoFile,
		DoString
	};

	LType tType = LType::DoFile;

	QQueue<QString> strList;

	static QString destoryId;

	static void hookFunction(lua_State* L, lua_Debug* ar);

	QMap<QString, QPair<size_t, void*>> shareData;
	QMutex shareMutex;

protected:
	void run()override;

signals:
	void errorMessage(QString message);
	void normalMessage(QString message);

	void tStarted(QString id);
	void tEnded(QString id);
};
