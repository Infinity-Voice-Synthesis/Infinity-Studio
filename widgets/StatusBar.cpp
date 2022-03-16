#include "StatusBar.h"
#include "utils/TextStateGetter.h"
#include "utils/ImageStateGetter.h"
#include "utils/Source.h"
#include "utils/Config.h"

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
	TextStateGetter::insert("SB_VirtualMachineState", [](const juce::String& id)->juce::String {return "Ready"; });
	TextStateGetter::insert("SB_VirtualMachineError", [](const juce::String& id)->juce::String {return "No Errors"; });
	TextStateGetter::insert("SB_Pattern", [](const juce::String& id)->juce::String {return "No Pattern"; });
	ImageStateGetter::insert("SB_Console", [](const juce::String& id)->juce::DrawableImage* {return Source::makeImage(Config::tsFull("SB_Console", "icon-normal")); });
	ImageStateGetter::insert("SB_VirtualMachineError", [](const juce::String& id)->juce::DrawableImage* {return Source::makeImage(Config::tsFull("SB_VirtualMachineError", "icon-normal")); });

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

	StatusBarButton* button = dynamic_cast<StatusBarButton*>(this->getItemComponent(item.first));
	button->setButtonText(item.second.textFunc(item.second.id));
	button->resized();
}

void StatusBar::refreshItemIcon(const juce::String& id)
{
	std::pair<int, const StatusBarFactory::Item&> item = this->statusBarFactory->findItem(id);
	if (item.first == -1) {
		return;
	}

	StatusBarButton* button = dynamic_cast<StatusBarButton*>(this->getItemComponent(item.first));
	button->setButtonNormalIcon(std::move(item.second.iconFunc(item.second.id)));
	button->setButtonToggledIcon(std::move(item.second.iconFunc(item.second.id)));
}