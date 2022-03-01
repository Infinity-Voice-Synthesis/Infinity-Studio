#pragma once

#include <JuceHeader.h>

class Menu
{
public:
	Menu();
	
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
		juce::String text;
		bool enable = true;

		bool checked = false;

		juce::String groupId;
		bool selected = false;

		juce::String subId;
	};

	virtual const juce::String getMenuId() = 0;
	virtual const std::vector<Item> getItemList() = 0;

private:
	friend class MenuManager;
	friend class MenuStateMachine;

	juce::String id;
	std::vector<Item> items;

};

