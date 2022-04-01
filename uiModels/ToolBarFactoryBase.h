#pragma once
#include <JuceHeader.h>

class ToolBarFactoryBase :
    public juce::ToolbarItemFactory
{
public:
    ToolBarFactoryBase();
    ~ToolBarFactoryBase();

    void getAllToolbarItemIds(juce::Array<int>& ids)override;
    void getDefaultItemSet(juce::Array<int>& ids)override;
    juce::ToolbarItemComponent* createItem(int itemId)override;

    int getItemIndex(const juce::String& id);

protected:
    struct Item {
        enum class Type {
            Normal = 0,
            Separator = 1,
            Spacer = 2,
            FlexibleSpacer = 3
        };
        Type type = Type::Normal;
        juce::String id;

        bool toggleAble = true;

        std::function<bool(void)> stateGetter = []()->bool {return false; };
        std::function<void(void)> callBack = [] {};
    };

    std::vector<Item> toolBarItems;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ToolBarFactoryBase)
};

