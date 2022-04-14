#include "ConsoleToolBar.h"
#include "utils/CallBackManager.h"

ConsoleToolBar::ConsoleToolBar()
{
	this->consoleToolBarFactory = std::make_unique<ConsoleToolBarFactory>();
}

ConsoleToolBar::~ConsoleToolBar()
{

}

void ConsoleToolBar::init(std::function<void(void)> codeRunStop)
{
	this->codeRunStop = codeRunStop;
	CallBackManager::set<void(juce::StringRef)>(
		"lambda_ConsoleToolBar_RSActived_const_juce::String&",
		[this](juce::StringRef id) {this->RunStopChanged(id); }
	);

	this->setVertical(true);
	this->setStyle(juce::Toolbar::ToolbarItemStyle::iconsWithText);
	this->setEditingActive(false);
	this->addDefaultItems(*(this->consoleToolBarFactory));
}

void ConsoleToolBar::RunStopChanged(juce::StringRef /*id*/)
{
	this->codeRunStop();
}
