#pragma once

#include <JuceHeader.h>

class Menu
{
public:
	Menu();
	void init();
	
protected:
	struct Item
	{
		enum class Type
		{
			Normal = 0,
			Checkable = 1,
			SingleSelectable = 2,
			SubMenu = 3,
			Separator = 4,
			Head = 5
		};
		Type type = Type::Normal;
		juce::String id;
		bool enable = true;
		bool checked = false;
		juce::String groupId;
		juce::String subId;
	};

	virtual const juce::String getMenuId() const = 0;
	virtual const std::vector<Item> getItemList() const = 0;

private:
	friend class MenuManager;
	friend class MenuStateMachine;

	juce::String id;
	std::vector<Item> items;

};

