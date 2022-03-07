#include "MenuBarModel.h"
#include "utils/Config.h"
#include "menus/utils/MenuManager.h"

MenuBarModel::MenuBarModel()
{

}

MenuBarModel::~MenuBarModel()
{

}

void MenuBarModel::init()
{
	this->menuBarNames.clear();
	for (auto& s : this->menuBarItemId) {
		this->menuBarNames.add(Config::tr(std::move(s)));
	}
	this->menuItemsChanged();
}

juce::StringArray MenuBarModel::getMenuBarNames()
{
	return this->menuBarNames;
}

juce::PopupMenu MenuBarModel::getMenuForIndex(int topLevelMenuIndex, const juce::String& menuName)
{
	return MenuManager::get(this->menuBarItemId[topLevelMenuIndex]);
}

void MenuBarModel::menuItemSelected(int menuItemID, int topLevelMenuIndex)
{
}