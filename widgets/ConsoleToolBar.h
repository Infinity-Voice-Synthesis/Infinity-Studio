#pragma once
#include <JuceHeader.h>
#include "uiModels/ConsoleToolBarFactory.h"

class ConsoleToolBar final :
    public juce::Toolbar
{
public:
    ConsoleToolBar();
    ~ConsoleToolBar();

    void init(std::function<void(void)> codeRunStop);

    void runStopChanged(juce::StringRef id);
    void changeRunStop(bool isRunning);

private:
    std::unique_ptr<ConsoleToolBarFactory> consoleToolBarFactory;
    std::function<void(void)> codeRunStop;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ConsoleToolBar)
};

