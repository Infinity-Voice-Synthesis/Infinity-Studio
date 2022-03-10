#pragma once
#include <JuceHeader.h>
#include "uiModels/StatusBarFactory.h"

class StatusBar final :
    public juce::Toolbar
{
public:
    StatusBar();
    ~StatusBar();

    void init();

private:
    std::unique_ptr<StatusBarFactory> statusBarFactory;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StatusBar);
};

