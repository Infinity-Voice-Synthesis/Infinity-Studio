#pragma once
#include <JuceHeader.h>

class NoteView final :
    public juce::Component
{
public:
    NoteView();
    ~NoteView();

protected:
    void paint(juce::Graphics& g)override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NoteView)

};

