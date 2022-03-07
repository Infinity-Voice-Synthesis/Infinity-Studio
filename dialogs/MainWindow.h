#pragma once

#include <JuceHeader.h>
#include "widgets/CentralWidget.h"

class MainWindow final :
    public juce::DocumentWindow
{
public:
    MainWindow(juce::String name);

    void init();

    void closeButtonPressed() override;

private:
    std::unique_ptr<CentralWidget> central;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
};