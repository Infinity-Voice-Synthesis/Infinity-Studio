#pragma once
#include <JuceHeader.h>

#include "Lua/lua.hpp"

#include "llibs/ILLibs.h"

class LThread : public juce::Thread
{
public:
	LThread();
	~LThread();

	static void init(
		std::function<void(juce::String)> errorMessage,
		std::function<void(juce::String)> normalMessage,
		std::function<void(juce::String)> tStarted,
		std::function<void(juce::String)> tEnded
	);

	bool doFile(juce::String name);
	bool doString(juce::String str);

	bool setId(juce::String id);
	juce::String getId();

	bool destory(juce::String id);

	void beginGlobalTable();
	void endGlobalTable(juce::String name);

	void beginTable(juce::String name);
	void endTable();

	void addFunction(juce::String name, lua_CFunction function);

	void loadUtils();

	static void set_destory(juce::String destoryId);

	bool checkShare(juce::String key);
	void* newShare(juce::String key, size_t size);
	bool removeShare(juce::String key);
	void* getShare(juce::String key);
	size_t sizeShare(juce::String key);
	bool clearShare();
	juce::StringArray listShare();
	void lockShare();
	void unlockShare();

private:
	lua_State* lstate = nullptr;

	juce::String lFileName;
	juce::String Id;
	juce::ReadWriteLock idMutex;

	enum class LType {
		DoFile,
		DoString
	};

	LType tType = LType::DoFile;

	std::queue<juce::String> strList;

	static juce::String destoryId;

	static void hookFunction(lua_State* L, lua_Debug* ar);

	juce::HashMap<juce::String, std::pair<size_t, void*>> shareData;
	std::mutex shareMutex;

	void run()override;

private:
	static std::function<void(juce::String)> errorMessage;
	static std::function<void(juce::String)> normalMessage;

	static std::function<void(juce::String)> tStarted;
	static std::function<void(juce::String)> tEnded;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LThread)
};
