#include "ToolBarFactory.h"
#include "utils/Config.h"
#include "utils/Source.h"
#include "menus/utils/MenuManager.h"

ToolBarFactory::ToolBarFactory()
{

}

ToolBarFactory::~ToolBarFactory()
{

}

void ToolBarFactory::getAllToolbarItemIds(juce::Array<int>& ids)
{
	for (int i = 0; i < this->toolBarItems.size(); i++) {
		Item& item = this->toolBarItems[i];
		switch (item.type)
		{
		case Item::Type::Normal:
		{
			ids.add(i + 1);
			break;
		}
		case Item::Type::Separator:
		{
			ids.add(juce::ToolbarItemFactory::SpecialItemIds::separatorBarId);
			break;
		}
		case Item::Type::Spacer:
		{
			ids.add(juce::ToolbarItemFactory::SpecialItemIds::spacerId);
			break;
		}
		case Item::Type::FlexibleSpacer:
		{
			ids.add(juce::ToolbarItemFactory::SpecialItemIds::flexibleSpacerId);
			break;
		}
		}
	}
}

void ToolBarFactory::getDefaultItemSet(juce::Array<int>& ids)
{
	this->getAllToolbarItemIds(ids);
}

juce::ToolbarItemComponent* ToolBarFactory::createItem(int itemId)
{
	Item& item = this->toolBarItems[static_cast<long long>(itemId) - 1];
	juce::ToolbarButton* button = new juce::ToolbarButton(
		itemId,
		Config::tr(item.id),
		std::move(Source::makeImage(Config::tsFull(item.id, "icon-normal"))),
		std::move(Source::makeImage(Config::tsFull(item.id, "icon-checked")))
	);
	button->setToggleable(true);
	button->setToggleState(MenuManager::isActived(item.id), juce::NotificationType::dontSendNotification);

	bool topConnect = (((static_cast<long long>(itemId) - 1) - 1) >= 0) &&
		(this->toolBarItems[static_cast<long long>(itemId) - 1 - 1].type == Item::Type::Normal);
	bool bottomConnect = (((static_cast<long long>(itemId) - 1) + 1) < static_cast<long long>(this->toolBarItems.size())) &&
		(this->toolBarItems[static_cast<long long>(itemId) - 1 + 1].type == Item::Type::Normal);
	button->setConnectedEdges(
		(topConnect ? juce::Button::ConnectedEdgeFlags::ConnectedOnTop : 0) |
		(bottomConnect ? juce::Button::ConnectedEdgeFlags::ConnectedOnBottom : 0)
	);

	button->onClick = [item] {MenuManager::actived(item.id); };
	return button;
}

int ToolBarFactory::getItemIndex(const juce::String& id)
{
	for (int i = 0; i < this->toolBarItems.size(); i++) {
		Item& item = this->toolBarItems[i];
		if (item.id == id) {
			return i;
		}
	}
	return -1;
}