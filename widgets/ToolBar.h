#pragma once
#include <JuceHeader.h>
#include "uiModels/ToolBarFactory.h"

class ToolBar final :
    public juce::Toolbar
{
public:
    ToolBar();
    ~ToolBar();

    void init();

private:
    std::unique_ptr<ToolBarFactory> toolBarFactory;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ToolBar)
};

