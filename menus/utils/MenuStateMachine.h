#pragma once

#include <JuceHeader.h>
#include "Menu.h"

class MenuStateMachine final
{
public:
	MenuStateMachine() {};
	void add(Menu::Item* item);
	void connect(juce::String id, std::function<void(bool)> Func);
	void reset(juce::String id);
	void actived(juce::String id);
	bool isActived(juce::String id);
	void setActiveCallBack(std::function<void(juce::StringRef, bool)> activeCallBack);
private:
	juce::HashMap<juce::String, std::pair<Menu::Item*, std::function<void(bool)>>> list;

	std::function<void(juce::StringRef, bool)> activeCallBack = [](juce::StringRef, bool) {};

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MenuStateMachine)
};

