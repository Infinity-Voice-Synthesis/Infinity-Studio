#pragma once
#include <JuceHeader.h>
#include "Lua/lua.hpp"
#include "llibs/ILLibs.h"

class LThread : public juce::Thread
{
public:
	LThread();
	~LThread();

	void connect(
		std::function<void(const juce::String&)> errorMessage,
		std::function<void(const juce::String&)> normalMessage,
		std::function<void(const juce::String&)> tStarted,
		std::function<void(const juce::String&)> tEnded
	);
	void disconnect();

	static void set_destory(const juce::String& destoryId);

	bool doFile(const juce::String& name);
	bool doString(const juce::String& str);

	bool setId(const juce::String& id);
	const juce::String& getId();

	bool destory(const juce::String& id);

	void beginGlobalTable();
	void endGlobalTable(const juce::String& name);

	void beginTable(const juce::String& name);
	void endTable();

	void addFunction(const juce::String& name, lua_CFunction function);

	void loadUtils();

	bool checkShare(const juce::String& key);
	void* newShare(const juce::String& key, size_t size);
	bool removeShare(const juce::String& key);
	void* getShare(const juce::String& key);
	size_t sizeShare(const juce::String& key);
	bool clearShare();
	juce::StringArray listShare();
	void lockShare();
	void unlockShare();

private:
	static juce::String destoryId;

	static void hookFunction(lua_State* L, lua_Debug* ar);

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

	juce::HashMap<juce::String, std::pair<size_t, void*>> shareData;
	std::mutex shareMutex;

	void run()override;

private:
	std::function<void(const juce::String&)> errorMessage;
	std::function<void(const juce::String&)> normalMessage;

	std::function<void(const juce::String&)> tStarted;
	std::function<void(const juce::String&)> tEnded;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LThread)
};
