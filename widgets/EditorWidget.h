#pragma once
#include <JuceHeader.h>

class EditorWidget :
    public juce::Component
{
public:
    EditorWidget();
    ~EditorWidget();

    void init();

private:
    std::unique_ptr<juce::StretchableLayoutManager> stretchableManager;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EditorWidget)
};

