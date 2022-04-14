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
		std::function<void(juce::StringRef)> errorMessage,
		std::function<void(juce::StringRef)> normalMessage,
		std::function<void(juce::StringRef)> tStarted,
		std::function<void(juce::StringRef)> tEnded
	);
	void disconnect();

	bool doFile(juce::StringRef name);
	bool doString(juce::StringRef str);

	bool setId(juce::StringRef id);
	juce::StringRef getId();

	bool destory(juce::StringRef id);

	void beginGlobalTable();
	void endGlobalTable(juce::StringRef name);

	void beginTable(juce::StringRef name);
	void endTable();

	void addFunction(juce::StringRef name, lua_CFunction function);

	void loadUtils();

	bool checkShare(juce::StringRef key);
	void* newShare(juce::StringRef key, size_t size);
	bool removeShare(juce::StringRef key);
	void* getShare(juce::StringRef key);
	size_t sizeShare(juce::StringRef key);
	bool clearShare();
	juce::StringArray listShare();
	void lockShare();
	void unlockShare();

private:
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
	std::function<void(juce::StringRef)> errorMessage;
	std::function<void(juce::StringRef)> normalMessage;

	std::function<void(juce::StringRef)> tStarted;
	std::function<void(juce::StringRef)> tEnded;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LThread)
};
