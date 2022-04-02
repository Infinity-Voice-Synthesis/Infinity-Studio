#pragma once
#include <JuceHeader.h>
#include "ConsoleToolBar.h"

class ConsoleWidget :
    public juce::Component
{
public:
    ConsoleWidget();
    ~ConsoleWidget();

    void init();

    void resized()override;

private:
    std::unique_ptr<juce::CodeEditorComponent> codeEditor;
    std::unique_ptr<ConsoleToolBar> consoleToolBar;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ConsoleWidget)
};

