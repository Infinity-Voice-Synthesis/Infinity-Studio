#include "StatusBar.h"
#include "utils/Source.h"
#include "utils/Config.h"
#include "utils/Utils.h"
#include "utils/CallBackManager.h"

StatusBar::StatusBar() :
	Toolbar()
{
	this->statusBarFactory = std::make_unique<StatusBarFactory>();
}

StatusBar::~StatusBar()
{

}

static bool __VMIsRunning = false;

void StatusBar::init()
{
	bool& VMIsRunning = ::__VMIsRunning;

	Utils::getTSG().insert("SB_VirtualMachineState", [&VMIsRunning](juce::StringRef id)->juce::String {return Config::tr(VMIsRunning ? "VMSB_Busy" : "VMSB_Free"); });
	Utils::getTSG().insert("SB_VirtualMachineError", [](juce::StringRef id)->juce::String {return Config::tr("VMEB_NoErrors"); });
	Utils::getTSG().insert("SB_Pattern", [](juce::StringRef id)->juce::String {return Config::tr("PB_NoPatterns"); });
	Utils::getISG().insert("SB_Console", [](juce::StringRef id)->juce::DrawableImage* {return Source::makeImage(Config::tsFull("SB_Console", "icon-normal")); });

	CallBackManager::set<void(bool*)>(
		"lambda_StatusBar_VMRunning_bool&",
		[&VMIsRunning](bool* isRunning) {*isRunning = VMIsRunning; }
		);

	CallBackManager::set<void(bool)>(
		"lambda_StatusBar_VMStartStop_bool",
		[&VMIsRunning, this](bool startStop) 
		{
			VMIsRunning = startStop;
			CallBackManager::call<void(bool)>("lambda_ConsoleWidget_CodeRunStop_bool", startStop);
			this->refreshItemText("SB_VirtualMachineState"); 
		}
	);

	this->setVertical(false);
	this->setStyle(juce::Toolbar::ToolbarItemStyle::iconsOnly);
	this->setEditingActive(false);
	this->addDefaultItems(*(this->statusBarFactory));
}

void StatusBar::refreshItemText(juce::StringRef id)
{
	const juce::MessageManagerLock lock;
	std::pair<int,const StatusBarFactory::Item&> item = this->statusBarFactory->findItem(id);
	if (item.first == -1) {
		return;
	}

	StatusBarButton* button = dynamic_cast<StatusBarButton*>(this->getItemComponent(item.first));
	button->setButtonText(item.second.textFunc(id));
	this->updateAllItemPositions(true);
}

void StatusBar::refreshItemIcon(juce::StringRef id)
{
	const juce::MessageManagerLock lock;
	std::pair<int, const StatusBarFactory::Item&> item = this->statusBarFactory->findItem(id);
	if (item.first == -1) {
		return;
	}

	StatusBarButton* button = dynamic_cast<StatusBarButton*>(this->getItemComponent(item.first));
	button->setButtonNormalIcon(std::move(item.second.iconFunc(id)));
	button->setButtonToggledIcon(std::move(item.second.iconFunc(id)));
	this->updateAllItemPositions(true);
}