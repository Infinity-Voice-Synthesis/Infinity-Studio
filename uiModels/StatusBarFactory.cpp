#include "StatusBarFactory.h"
#include "utils/Config.h"
#include "utils/Source.h"

std::function<bool(const StatusBarFactory::Item::ShowType&, const StatusBarFactory::Item::ShowType&)> StatusBarFactory::Item::stateCheckFunc
= [](const StatusBarFactory::Item::ShowType& arg1, const StatusBarFactory::Item::ShowType& arg2) ->bool
{
	return static_cast<uint64_t>(arg1) & static_cast<uint64_t>(arg2);
};

std::function<juce::String(const juce::String&)> StatusBarFactory::Item::textFunc
= [](const juce::String& id)->juce::String {return juce::String(); };

std::function<std::unique_ptr<juce::DrawableImage>(const juce::String&)> StatusBarFactory::Item::iconFunc
= [](const juce::String& id)->std::unique_ptr<juce::DrawableImage> {return std::make_unique<juce::DrawableImage>(); };

StatusBarFactory::StatusBarFactory() :
	ToolbarItemFactory()
{

}

StatusBarFactory::~StatusBarFactory()
{

}

void StatusBarFactory::getAllToolbarItemIds(juce::Array<int>& ids)
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

void StatusBarFactory::getDefaultItemSet(juce::Array<int>& ids)
{
	this->getAllToolbarItemIds(ids);
}

juce::ToolbarItemComponent* StatusBarFactory::createItem(int itemId)
{
	Item& item = this->toolBarItems[static_cast<long long>(itemId) - 1];
	juce::ToolbarButton* button = new juce::ToolbarButton(
		itemId,
		(
			Item::stateCheckFunc(item.showType, Item::ShowType::Text)
			? Config::tr(StatusBarFactory::Item::textFunc(item.id))
			: ""
			),
		(
			Item::stateCheckFunc(item.showType, Item::ShowType::Icon)
			? std::move(StatusBarFactory::Item::iconFunc(item.id))
			: std::make_unique<juce::DrawableImage>()
			),
		(
			Item::stateCheckFunc(item.showType, Item::ShowType::Icon)
			? std::move(StatusBarFactory::Item::iconFunc(item.id))
			: std::make_unique<juce::DrawableImage>()
			)
	);
	button->setToggleable(false);

	bool leftConnect = (((static_cast<long long>(itemId) - 1) - 1) >= 0) &&
		(this->toolBarItems[static_cast<long long>(itemId) - 1 - 1].type == Item::Type::Normal);
	bool rightConnect = (((static_cast<long long>(itemId) - 1) + 1) < static_cast<long long>(this->toolBarItems.size())) &&
		(this->toolBarItems[static_cast<long long>(itemId) - 1 + 1].type == Item::Type::Normal);
	button->setConnectedEdges(
		(leftConnect ? juce::Button::ConnectedEdgeFlags::ConnectedOnLeft : 0) |
		(rightConnect ? juce::Button::ConnectedEdgeFlags::ConnectedOnRight : 0)
	);

	button->onClick = [item] {item.activeFunc(item.id); };
	return button;
}
