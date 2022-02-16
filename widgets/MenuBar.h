#pragma once
#include "JuceHeader.h"

class MenuBar :
    public juce::MenuBarComponent
{
public:
    MenuBar();

    void init();
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MenuBar)
};

