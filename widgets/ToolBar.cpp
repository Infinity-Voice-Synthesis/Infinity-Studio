#include "ToolBar.h"

ToolBar::ToolBar()
{
	this->toolBarFactory = std::make_unique<ToolBarFactory>();
}

ToolBar::~ToolBar()
{
}

void ToolBar::init()
{
	this->setVertical(true);
	this->setStyle(juce::Toolbar::ToolbarItemStyle::iconsOnly);
	this->setEditingActive(false);
	this->addItem(*(this->toolBarFactory), 1, 1);
}
