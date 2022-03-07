#include "ToolBarFactory.h"
#include "utils/Config.h"

ToolBarFactory::ToolBarFactory()
{

}

ToolBarFactory::~ToolBarFactory()
{

}

void ToolBarFactory::getAllToolbarItemIds(juce::Array<int>& ids)
{
	ids = { 1 };
}

void ToolBarFactory::getDefaultItemSet(juce::Array<int>& ids)
{
}

juce::ToolbarItemComponent* ToolBarFactory::createItem(int itemId)
{
	return new juce::ToolbarButton(itemId, "", std::make_unique<juce::DrawableImage>(), std::make_unique<juce::DrawableImage>());
}
