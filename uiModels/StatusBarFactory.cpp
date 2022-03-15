#include "StatusBarFactory.h"
#include "utils/Config.h"
#include "utils/Source.h"
#include "utils/TextStateGetter.h"

StatusBarFactory::Item::ItemTextFunction StatusBarFactory::Item::textFunc
= [](const juce::String& id)->juce::String {return L"²âÊÔ×Ö·û"/*TextStateGetter::get(id)*/; };

StatusBarFactory::StatusBarFactory() :
	ToolbarItemFactory()
{

}

StatusBarFactory::~StatusBarFactory()
{

}

void StatusBarFactory::getAllToolbarItemIds(juce::Array<int>& ids)
{
	for (int i = 0; i < this->statusBarItems.size(); i++) {
		Item& item = this->statusBarItems[i];
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

void StatusBarFactory::getDefaultItemSet(juce::Array<int>& ids)
{
	this->getAllToolbarItemIds(ids);
}

juce::ToolbarItemComponent* StatusBarFactory::createItem(int itemId)
{
	Item& item = this->statusBarItems[static_cast<long long>(itemId) - 1];
	StatusBarButton* button = new StatusBarButton(
		itemId,
		item.showType,
		(
			StatusBarButton::stateCheckFunc(item.showType, StatusBarButton::ShowType::Text)
			? Config::tr(StatusBarFactory::Item::textFunc(item.id))
			: ""
			),
		(
			StatusBarButton::stateCheckFunc(item.showType, StatusBarButton::ShowType::Icon)
			? std::move(Source::makeImage(Config::tsFull(item.id, "icon-normal")))
			: std::make_unique<juce::DrawableImage>()
			),
		(
			StatusBarButton::stateCheckFunc(item.showType, StatusBarButton::ShowType::Icon)
			? std::move(Source::makeImage(Config::tsFull(item.id, "icon-normal")))
			: std::make_unique<juce::DrawableImage>()
			)
	);
	button->setToggleable(false);

	bool leftConnect = (((static_cast<long long>(itemId) - 1) - 1) >= 0) &&
		(this->statusBarItems[static_cast<long long>(itemId) - 1 - 1].type == Item::Type::Normal);
	bool rightConnect = (((static_cast<long long>(itemId) - 1) + 1) < static_cast<long long>(this->statusBarItems.size())) &&
		(this->statusBarItems[static_cast<long long>(itemId) - 1 + 1].type == Item::Type::Normal);
	button->setConnectedEdges(
		(leftConnect ? juce::Button::ConnectedEdgeFlags::ConnectedOnLeft : 0) |
		(rightConnect ? juce::Button::ConnectedEdgeFlags::ConnectedOnRight : 0)
	);

	button->onClick = [item] {item.activeFunc(item.id); };
	return button;
}

std::pair<int, const StatusBarFactory::Item&> StatusBarFactory::findItem(const juce::String& id)
{
	for (int i = 0; i < this->statusBarItems.size(); i++) {
		Item& item = this->statusBarItems[i];
		if (item.id == id) {
			return std::make_pair(i, item);
		}
	}
	return std::make_pair(-1, *(this->_empty));
}
