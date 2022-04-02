#pragma once
#include <JuceHeader.h>
#include "uiModels/ConsoleToolBarFactory.h"

class ConsoleToolBar :
    public juce::Toolbar
{
public:
    ConsoleToolBar();
    ~ConsoleToolBar();

    void init();

    void RunStopChanged(const juce::String& id);

private:
    std::unique_ptr<ConsoleToolBarFactory> consoleToolBarFactory;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ConsoleToolBar)
};

