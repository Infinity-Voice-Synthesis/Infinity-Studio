#pragma once
#include "JuceHeader.h"

class ToolBarFactory final :
    public juce::ToolbarItemFactory
{
public:
    ToolBarFactory();
    ~ToolBarFactory();

    void getAllToolbarItemIds(juce::Array<int>& ids)override;
    void getDefaultItemSet(juce::Array<int>& ids)override;
    juce::ToolbarItemComponent* createItem(int itemId)override;
};

