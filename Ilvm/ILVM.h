#pragma once

#include <JuceHeader.h>
#include "LThread.h"
#include "llibs/ILLibs.h"
#include "llibs/DMH.h"

#define ILVM_COPYRIGHT "Infinity Studio Script Interpreter 1.0.0" "  Copyright (C) 2019-2022 Infinity Synthesis Team"

class ILVM final
{
public:
	ILVM();
	~ILVM();

private:
	static std::unique_ptr<ILVM> vm;

	std::vector<LThread*> threads;
	std::vector<LThread*> threads_bin;
	LThread* mainThread = nullptr;

	const juce::String mainId = "_main";
	const juce::String destoryId = "_destory";

	static juce::String outStrTemp;

	bool isSafeMode = false;

	void VMPushOptionalFunctions(LThread* thread);
	void VMPushAllFunctions(LThread* thread);

	static void lStdOut(lua_State* L, const char* data, size_t size);
	static void lStdOutLine(lua_State* L);
	static void lStdOutErr(lua_State* L, const char* format, const char* data);

public:
	void mainCritical();

	bool findThread(juce::String id);
	juce::StringArray getThreadList();
	bool createThread(juce::String id);
	bool removeThread(juce::String id);

	bool destoryThread(juce::String id);

	bool doStringOnThread(juce::String id, juce::String str);
	bool doFileOnThread(juce::String id, juce::String file);

	bool threadIsRunning(juce::String id);

	void flushBin();

	bool checkShare(juce::String id, juce::String key);
	void* newShare(juce::String id, juce::String key, size_t size);
	bool removeShare(juce::String id, juce::String key);
	void* getShare(juce::String id, juce::String key);
	size_t sizeShare(juce::String id, juce::String key);
	bool clearShare(juce::String id);
	juce::StringArray listShare(juce::String id);
	void lockShare(juce::String id);
	void unlockShare(juce::String id);

public:
	void on_commandsIn(juce::String command);

private:
	void on_threadStart(juce::String id);
	void on_threadStop(juce::String id);

private:
	static std::function<void(juce::String)> errorMessage;
	static std::function<void(juce::String)> normalMessage;
	static std::function<void(void)> clearMessage;

	static std::function<void(void)> mainStart;
	static std::function<void(void)> mainStop;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ILVM)
};
