#pragma once
#include <JuceHeader.h>
#include "utils/Config.h"
#include "menus/utils/MenuManager.h"

class MenuBar :
    public juce::MenuBarComponent,
    private juce::MenuBarModel
{
public:
    MenuBar();
    ~MenuBar();

    void init();
private:
    juce::StringArray getMenuBarNames()override;
    juce::PopupMenu getMenuForIndex(int topLevelMenuIndex, const juce::String& menuName)override;
    void menuItemSelected(int menuItemID, int topLevelMenuIndex)override;

    juce::StringArray menuBarNames;

    juce::StringArray menuBarItemId = {
        "MMB_File",
        "MMB_Edit",
        "MMB_View",
        "MMB_Modify",
        "MMB_Project",
        "MMB_Transport",
        "MMB_Automatic",
        "MMB_Config",
        "MMB_Misc"
    };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MenuBar)
};

