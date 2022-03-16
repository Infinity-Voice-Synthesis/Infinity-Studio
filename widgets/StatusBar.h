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

    void refreshItemText(const juce::String& id);
    void refreshItemIcon(const juce::String& id);

private:
    std::unique_ptr<StatusBarFactory> statusBarFactory;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StatusBar);
};

