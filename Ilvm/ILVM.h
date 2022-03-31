#pragma once

#include <JuceHeader.h>
#include "LThread.h"

constexpr auto ILVM_COPYRIGHT = "Infinity Studio Script Interpreter 1.0.0" "  Copyright (C) 2019-2022 Infinity Synthesis Team";

class ILVM final
{
public:
	ILVM();
	~ILVM();

	static void init(
		std::function<void(const juce::String&)> errorMessage,
		std::function<void(const juce::String&)> normalMessage,
		std::function<void(void)> clearMessage,
		std::function<void(void)> mainStart,
		std::function<void(void)> mainStop
	);
	static void destory();

private:
	static std::unique_ptr<ILVM> vm;

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
	void mainCritical();

	bool findThread(const juce::String& id);
	juce::StringArray getThreadList();
	bool createThread(const juce::String& id);
	bool removeThread(const juce::String& id);

	bool destoryThread(const juce::String& id);

	bool doStringOnThread(const juce::String& id, const juce::String& str);
	bool doFileOnThread(const juce::String& id, const juce::String& file);

	bool threadIsRunning(const juce::String& id);

	void flushBin();

	bool checkShare(const juce::String& id, const juce::String& key);
	void* newShare(const juce::String& id, const juce::String& key, size_t size);
	bool removeShare(const juce::String& id, const juce::String& key);
	void* getShare(const juce::String& id, const juce::String& key);
	size_t sizeShare(const juce::String& id, const juce::String& key);
	bool clearShare(const juce::String& id);
	juce::StringArray listShare(const juce::String& id);
	void lockShare(const juce::String& id);
	void unlockShare(const juce::String& id);

public:
	void on_commandsIn(const juce::String& command);

public:
	static void on_threadStart(const juce::String& id);
	static void on_threadStop(const juce::String& id);

	static std::function<void(const juce::String&)> errorMessage;
	static std::function<void(const juce::String&)> normalMessage;
	static std::function<void(void)> clearMessage;

	static std::function<void(void)> mainStart;
	static std::function<void(void)> mainStop;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ILVM)
};
