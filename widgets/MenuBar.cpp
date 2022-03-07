#include "MenuBar.h"

MenuBar::MenuBar()
{
	this->menuBarModel = std::make_unique<MenuBarModel>();
}

MenuBar::~MenuBar()
{
	this->setModel(nullptr);
}

void MenuBar::init()
{
	this->menuBarModel->init();
	this->setModel(this->menuBarModel.get());
}
