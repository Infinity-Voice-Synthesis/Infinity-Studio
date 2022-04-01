#pragma once
#include <JuceHeader.h>

class ConsoleWidget :
    public juce::Component
{
public:
    ConsoleWidget();
    ~ConsoleWidget();

private:
    std::unique_ptr<juce::CodeEditorComponent> codeEditor;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ConsoleWidget)
};

