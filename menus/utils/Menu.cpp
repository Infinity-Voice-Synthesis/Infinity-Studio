#include "Menu.h"
#include "MenuManager.h"

Menu::Menu()
{
}

void Menu::init()
{
	this->id = this->getMenuId();
	this->items = this->getItemList();
	MenuManager::add(this);
}
