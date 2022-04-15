#pragma once

#include <JuceHeader.h>
#include "LThread.h"

constexpr auto ILVM_COPYRIGHT = "Infinity Studio Script Interpreter 2.0.0" "  Copyright (C) 2019-2022 Infinity Synthesis Team";

class ILVM final
{
public:
	ILVM();
	~ILVM();

	static void init(
		std::function<void(juce::StringRef)> errorMessage,
		std::function<void(juce::StringRef)> normalMessage,
		std::function<void(void)> clearMessage,
		std::function<void(void)> mainStart,
		std::function<void(void)> mainStop
	);
	static void destory();

	static void run(juce::StringRef code);
	static void stop();

private:
	static std::unique_ptr<ILVM> vm;
	
	friend class LThread;

	std::vector<LThread*> threads;
	std::vector<LThread*> threads_bin;
	LThread* mainThread = nullptr;

	const juce::String mainId = "lua_main";
	const juce::String destoryId = "lua_destory";

	static juce::String outStrTemp;

	bool isSafeMode = false;

	void VMPushOptionalFunctions(LThread* thread);
	void VMPushAllFunctions(LThread* thread);

	static void lStdOut(lua_State* L, const char* data, size_t size);
	static void lStdOutLine(lua_State* L);
	static void lStdOutErr(lua_State* L, const char* format, const char* data);

public:
	bool findThread(juce::StringRef id);
	juce::StringArray getThreadList();
	bool createThread(juce::StringRef id);
	bool removeThread(juce::StringRef id);

	bool destoryThread(juce::StringRef id);

	bool doStringOnThread(juce::StringRef id, juce::StringRef str);
	bool doFileOnThread(juce::StringRef id, juce::StringRef file);

	bool threadIsRunning(juce::StringRef id);

	void flushBin();

	bool checkShare(juce::StringRef id, juce::StringRef key);
	void* newShare(juce::StringRef id, juce::StringRef key, size_t size);
	bool removeShare(juce::StringRef id, juce::StringRef key);
	void* getShare(juce::StringRef id, juce::StringRef key);
	size_t sizeShare(juce::StringRef id, juce::StringRef key);
	bool clearShare(juce::StringRef id);
	juce::StringArray listShare(juce::StringRef id);
	void lockShare(juce::StringRef id);
	void unlockShare(juce::StringRef id);

public:
	void commandsIn(juce::StringRef command);
	void mainCritical();

public:
	static void on_threadStart(juce::StringRef id);
	static void on_threadStop(juce::StringRef id);

	static std::function<void(juce::StringRef)> errorMessage;
	static std::function<void(juce::StringRef)> normalMessage;
	static std::function<void(void)> clearMessage;

	static std::function<void(void)> mainStart;
	static std::function<void(void)> mainStop;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ILVM)
};
