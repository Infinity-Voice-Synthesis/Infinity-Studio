#include "MenuBar.h"

MenuBar::MenuBar()
{

}

MenuBar::~MenuBar()
{
	this->setModel(nullptr);
}

void MenuBar::init()
{
	this->menuBarNames.clear();
	for (auto& s : this->menuBarItemId) {
		this->menuBarNames.add(Config::tr(std::move(s)));
	}
	this->menuItemsChanged();
	this->setModel(this);
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
