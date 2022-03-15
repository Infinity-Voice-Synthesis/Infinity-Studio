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
	this->setStyle(juce::Toolbar::ToolbarItemStyle::iconsOnly);
	this->setEditingActive(false);
	this->addDefaultItems(*(this->statusBarFactory));
}

void StatusBar::refreshItemText(const juce::String& id)
{
	std::pair<int,const StatusBarFactory::Item&> item = this->statusBarFactory->findItem(id);
	if (item.first == -1) {
		return;
	}

	this->getItemComponent(item.first)->setButtonText(item.second.textFunc(item.second.id));
}
