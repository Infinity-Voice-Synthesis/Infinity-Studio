#include "MenuBar.h"

MenuBar::MenuBar()
{

}

void MenuBar::init()
{
	this->menuBarNames.add("test1");
	this->menuBarNames.add("test2");
	this->menuItemsChanged();
}

juce::StringArray MenuBar::getMenuBarNames()
{
	return this->menuBarNames;
}

juce::PopupMenu MenuBar::getMenuForIndex(int topLevelMenuIndex, const juce::String& menuName)
{
	return juce::PopupMenu();
}

void MenuBar::menuItemSelected(int menuItemID, int topLevelMenuIndex)
{

}
