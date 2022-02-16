#pragma once
#include "JuceHeader.h"
#include "MenuBar.h"

class CentralWidget :
    public juce::Component
{
public:
    CentralWidget();

    void init();

    void resized()override;
    void paint(juce::Graphics& g)override;

private:
    std::unique_ptr<MenuBar> menuBar;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CentralWidget)
};

