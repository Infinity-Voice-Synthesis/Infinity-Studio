#include "StatusBar.h"

StatusBar::StatusBar() :
	Toolbar()
{
	this->statusBarFactory = std::make_unique<StatusBarFactory>();
}

StatusBar::~StatusBar()
{

}

void StatusBar::init()
{
	this->setVertical(false);
	this->setStyle(juce::Toolbar::ToolbarItemStyle::iconsWithText);
	this->setEditingActive(false);
	this->addDefaultItems(*(this->statusBarFactory));
}
