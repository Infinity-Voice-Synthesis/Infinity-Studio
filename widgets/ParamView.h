#pragma once
#include <JuceHeader.h>

class ParamView final :
    public juce::Component
{
public:
    ParamView();
    ~ParamView();
    void setFocusOn(bool focusOn);

protected:
    void paint(juce::Graphics& g)override;

private:
    bool focusOnMode = true;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParamView)
};

