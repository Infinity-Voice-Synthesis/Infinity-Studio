#pragma once
#include <JuceHeader.h>
#include "ToolBarFactoryBase.h"

class ConsoleToolBarFactory :
    public ToolBarFactoryBase
{
public:
    ConsoleToolBarFactory();
    ~ConsoleToolBarFactory();

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ConsoleToolBarFactory)
};

