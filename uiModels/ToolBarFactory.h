#pragma once
#include <JuceHeader.h>
#include "ToolBarFactoryBase.h"

class ToolBarFactory final :
    public ToolBarFactoryBase
{
public:
    ToolBarFactory();
    ~ToolBarFactory();

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ToolBarFactory)
};

