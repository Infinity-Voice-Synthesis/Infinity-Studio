#pragma once
#include <JuceHeader.h>

class MenuBar :
    public juce::MenuBarComponent,
    private juce::MenuBarModel
{
public:
    MenuBar();

    void init();
private:
    juce::StringArray getMenuBarNames()override;
    juce::PopupMenu getMenuForIndex(int topLevelMenuIndex, const juce::String& menuName)override;
    void menuItemSelected(int menuItemID, int topLevelMenuIndex)override;

    juce::StringArray menuBarNames;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MenuBar)
};

