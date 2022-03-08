#pragma once
#include <JuceHeader.h>

class ToolBarFactory final :
    public juce::ToolbarItemFactory
{
public:
    ToolBarFactory();
    ~ToolBarFactory();

    void getAllToolbarItemIds(juce::Array<int>& ids)override;
    void getDefaultItemSet(juce::Array<int>& ids)override;
    juce::ToolbarItemComponent* createItem(int itemId)override;

    int getItemIndex(const juce::String& id);

private:
    struct Item {
        enum class Type {
            Normal = 0,
            Separator = 1,
            Spacer = 2,
            FlexibleSpacer = 3
        };
        Type type = Type::Normal;
        juce::String id;
    };

    std::vector<Item> toolBarItems = {
        {Item::Type::Normal,"MM_Explore_Mode"},
        {Item::Type::Normal,"MM_Edit_Mode"},
        {Item::Type::Spacer,""},
        {Item::Type::Normal,"MM_Track_View"},
        {Item::Type::Normal,"MM_Note_View"},
        {Item::Type::Normal,"MM_Parameter_View"},
        {Item::Type::Spacer,""},
        {Item::Type::Normal,"MM_Show_Notes"},
        {Item::Type::Normal,"MM_Show_Notes_and_Wave"},
        {Item::Type::Normal,"MM_Show_Notes_and_Pitch_Line"},
        {Item::Type::FlexibleSpacer,""},
        {Item::Type::Normal,"MM_Follow_the_Pointer"},
        {Item::Type::Normal,"MM_Loop"},
        {Item::Type::Normal,"MM_Play_or_Stop"},
        {Item::Type::Spacer,""}
    };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ToolBarFactory)
};

