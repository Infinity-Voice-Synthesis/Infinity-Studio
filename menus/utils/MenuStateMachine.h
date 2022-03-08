#pragma once

#include <JuceHeader.h>
#include "Menu.h"

class MenuStateMachine
{
public:
	void add(Menu::Item* item);
	void connect(juce::String id, std::function<void(bool)> Func);
	void reset(juce::String id);
	void actived(juce::String id);
	bool isActived(juce::String id);
	void setActiveCallBack(std::function<void(const juce::String&, bool)> activeCallBack);
private:
	juce::HashMap<juce::String, std::pair<Menu::Item*, std::function<void(bool)>>> list;

	std::function<void(const juce::String&, bool)> activeCallBack = [](const juce::String&, bool) {};
};

