#pragma once
#include <JuceHeader.h>
#include "widgets/StatusBarButton.h"

class StatusBarFactory final :
    public juce::ToolbarItemFactory
{
public:
    StatusBarFactory();
    ~StatusBarFactory();

    void getAllToolbarItemIds(juce::Array<int>& ids)override;
    void getDefaultItemSet(juce::Array<int>& ids)override;
    juce::ToolbarItemComponent* createItem(int itemId)override;

    struct Item {
        enum class Type {
            Normal = 0,
            Separator = 1,
            Spacer = 2,
            FlexibleSpacer = 3
        };

        Type type = Type::Normal;
        juce::String id;
        StatusBarButton::ShowType showType = StatusBarButton::ShowType::IconAndText;

        using ItemActiveFunction = std::function<void(const juce::String&)>;
        ItemActiveFunction activeFunc = [](const juce::String& id) {};

        using ItemTextFunction = std::function<juce::String(const juce::String&)>;
        static ItemTextFunction textFunc;
        using ItemIconFunction = std::function<std::unique_ptr<juce::Drawable>(const juce::String&)>;
        static ItemIconFunction iconFunc;
    };
private:

    std::vector<Item> statusBarItems = {
        {.type = Item::Type::Normal,.id = "SB_Console",.showType = StatusBarButton::ShowType::Icon},
        {.type = Item::Type::Normal,.id = "SB_VirtualMachineState",.showType = StatusBarButton::ShowType::Text},
        {.type = Item::Type::Normal,.id = "SB_VirtualMachineError",.showType = StatusBarButton::ShowType::IconAndText},
        {.type = Item::Type::FlexibleSpacer},
        {.type = Item::Type::Normal,.id = "SB_Pattern",.showType = StatusBarButton::ShowType::Text}
    };

    const std::unique_ptr<Item> _empty = std::make_unique<Item>();

public:
    std::pair<int, const Item&> findItem(const juce::String& id);

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StatusBarFactory)
};

