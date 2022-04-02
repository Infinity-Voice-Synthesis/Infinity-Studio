#include "ConsoleToolBar.h"
#include "utils/CallBackManager.h"

ConsoleToolBar::ConsoleToolBar()
{
	this->consoleToolBarFactory = std::make_unique<ConsoleToolBarFactory>();
}

ConsoleToolBar::~ConsoleToolBar()
{

}

void ConsoleToolBar::init()
{
	CallBackManager::set<void(const juce::String&)>(
		"lambda_ConsoleToolBar_RSActived_const_juce::String&",
		[this](const juce::String& id) {this->RunStopChanged(id); }
	);

	this->setVertical(true);
	this->setStyle(juce::Toolbar::ToolbarItemStyle::iconsWithText);
	this->setEditingActive(false);
	this->addDefaultItems(*(this->consoleToolBarFactory));
}

void ConsoleToolBar::RunStopChanged(const juce::String& /*id*/)
{
	
}
