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
		[this](juce::StringRef id) {this->runStopChanged(id); }
	);

	this->setVertical(true);
	this->setStyle(juce::Toolbar::ToolbarItemStyle::iconsWithText);
	this->setEditingActive(false);
	this->addDefaultItems(*(this->consoleToolBarFactory));
}

void ConsoleToolBar::runStopChanged(juce::StringRef /*id*/)
{
	this->codeRunStop();
}

void ConsoleToolBar::changeRunStop(bool isRunning)
{
	int index = this->consoleToolBarFactory->getItemIndex("CTB_RunStop");
	if (index == -1) {
		return;
	}

	juce::MessageManagerLock lock;
	this->getItemComponent(index)->setToggleState(isRunning, juce::NotificationType::dontSendNotification);
}