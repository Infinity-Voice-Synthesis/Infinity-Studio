#pragma once
#include <JuceHeader.h>
#include "uiModels/ConsoleToolBarFactory.h"

class ConsoleToolBar :
    public juce::Toolbar
{
public:
    ConsoleToolBar();
    ~ConsoleToolBar();

    void init(std::function<void(void)> codeRunStop);

    void RunStopChanged(juce::StringRef id);

private:
    std::unique_ptr<ConsoleToolBarFactory> consoleToolBarFactory;
    std::function<void(void)> codeRunStop;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ConsoleToolBar)
};

