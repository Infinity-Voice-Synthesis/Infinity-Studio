#include "ToolBar.h"
#include "menus/utils/MenuManager.h"

ToolBar::ToolBar() :
	Toolbar()
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
	this->addDefaultItems(*(this->toolBarFactory));

	MenuManager::setActiveCallBack(
		[this](const juce::String& id, bool isActived)->void
		{
			int index = this->toolBarFactory->getItemIndex(id);
			if (index == -1) {
				return;
			}

			this->getItemComponent(index)->setToggleState(isActived, juce::NotificationType::dontSendNotification);
		}
	);
}
