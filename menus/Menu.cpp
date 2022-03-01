#include "Menu.h"
#include "MenuManager.h"

Menu::Menu()
{
	this->id = this->getMenuId();
	this->items = this->getItemList();
	MenuManager::add(std::shared_ptr<Menu>(this));
}
