#pragma once
#include <JuceHeader.h>
#include "Menu.h"
#include "MenuStateMachine.h"
#include "utils/Config.h"

class MenuManager final
{
	static std::unique_ptr<MenuManager> _menuManager;
	static std::unique_ptr<MenuStateMachine> _menuStateMachine;

public:
	MenuManager();
	~MenuManager();

	static void init();
	static void destory();

	static void add(Menu* menu);

	static juce::PopupMenu get(juce::String id);

	static void actived(juce::String id);
	static bool isActived(juce::String id);

	static void setActiveCallBack(std::function<void(juce::StringRef, bool)> activeCallBack);

	static void connect(juce::String id, std::function<void(bool)> Func);
	static void reset(juce::String id);

private:
	juce::HashMap<juce::String, Menu*> list;

	juce::PopupMenu DFS(juce::String id);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MenuManager)
};

