#pragma once
#include <JuceHeader.h>

class TrackView final :
    public juce::Component
{
public:
    TrackView();
    ~TrackView();

protected:
    void paint(juce::Graphics& g)override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TrackView)
};

