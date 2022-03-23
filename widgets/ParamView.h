#pragma once
#include <JuceHeader.h>

class ParamView final :
    public juce::Component
{
public:
    ParamView();
    ~ParamView();

protected:
    void paint(juce::Graphics& g)override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParamView)

};

