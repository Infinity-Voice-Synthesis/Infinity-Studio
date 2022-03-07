#pragma once
#include <JuceHeader.h>
#include "uiModels/MenuBarModel.h"

class MenuBar final :
    public juce::MenuBarComponent
{
public:
    MenuBar();
    ~MenuBar();

    void init();

private:
    std::unique_ptr<MenuBarModel> menuBarModel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MenuBar)
};

