#include "StatusBarFactory.h"
#include "utils/Config.h"
#include "utils/Source.h"
#include "utils/Utils.h"
#include "utils/CallBackManager.h"

StatusBarFactory::Item::ItemTextFunction StatusBarFactory::Item::textFunc
= [](juce::StringRef id)->juce::String {return Utils::getTSG().get(id); };
StatusBarFactory::Item::ItemIconFunction StatusBarFactory::Item::iconFunc
= [](juce::StringRef id)->std::unique_ptr<juce::Drawable> {return std::unique_ptr<juce::Drawable>(Utils::getISG().get(id)); };

StatusBarFactory::StatusBarFactory() :
	ToolbarItemFactory()
{
	this->statusBarItems = {
		{.type = Item::Type::Normal,.id = "SB_Console",.showType = StatusBarButton::ShowType::Icon,
		.activeFunc = [](juce::StringRef) {CallBackManager::call<void(void)>("lambda_CentralWidget_ConsoleChange_void"); }},
		{.type = Item::Type::Normal,.id = "SB_VirtualMachineState",.showType = StatusBarButton::ShowType::Text,
		.activeFunc = [](juce::StringRef) {CallBackManager::call<void(void)>("lambda_CentralWidget_VMStateChange_void"); }},
		{.type = Item::Type::Normal,.id = "SB_VirtualMachineError",.showType = StatusBarButton::ShowType::Text,
		.activeFunc = [](juce::StringRef) {CallBackManager::call<void(void)>("lambda_CentralWidget_VMErrorShow_void"); }},
		{.type = Item::Type::FlexibleSpacer},
		{.type = Item::Type::Normal,.id = "SB_Pattern",.showType = StatusBarButton::ShowType::Text}
	};
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
			? std::move(StatusBarFactory::Item::iconFunc(item.id))
			: std::make_unique<juce::DrawableImage>()
			),
		(
			StatusBarButton::stateCheckFunc(item.showType, StatusBarButton::ShowType::Icon)
			? std::move(StatusBarFactory::Item::iconFunc(item.id))
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

std::pair<int, const StatusBarFactory::Item&> StatusBarFactory::findItem(juce::StringRef id)
{
	for (int i = 0; i < this->statusBarItems.size(); i++) {
		Item& item = this->statusBarItems[i];
		if (item.id == id) {
			return std::make_pair(i, item);
		}
	}
	return std::make_pair(-1, *(this->_empty));
}
