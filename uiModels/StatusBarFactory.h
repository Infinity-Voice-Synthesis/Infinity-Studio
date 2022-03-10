#pragma once
#include <JuceHeader.h>

class StatusBarFactory final :
    public juce::ToolbarItemFactory
{
public:
    StatusBarFactory();
    ~StatusBarFactory();

    void getAllToolbarItemIds(juce::Array<int>& ids)override;
    void getDefaultItemSet(juce::Array<int>& ids)override;
    juce::ToolbarItemComponent* createItem(int itemId)override;

private:
    struct Item {
        enum class Type {
            Normal = 0,
            Separator = 1,
            Spacer = 2,
            FlexibleSpacer = 3
        };
        enum class ShowType {
            Hide = 0,
            Icon = 1 << 0,
            Text = 1 << 1,
            IconAndText = 1 << 0 | 1 << 1
        };
        Type type = Type::Normal;
        juce::String id;
        ShowType showType = ShowType::IconAndText;
        std::function<void(const juce::String&)> activeFunc = [](const juce::String& id) {};

        static std::function<juce::String(const juce::String&)> textFunc;
        static std::function<std::unique_ptr<juce::DrawableImage>(const juce::String&)> iconFunc;

        static std::function<bool(const ShowType&, const ShowType&)> stateCheckFunc;
    };

    std::vector<Item> toolBarItems = {
        {.type = Item::Type::Normal,.id = "SB_Console",.showType = Item::ShowType::Icon},
        {.type = Item::Type::Normal,.id = "SB_VirtualMachineState",.showType = Item::ShowType::Icon},
        {.type = Item::Type::FlexibleSpacer},
        {.type = Item::Type::Normal,.id = "SB_Pattern",.showType = Item::ShowType::Text}
    };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StatusBarFactory)
};

