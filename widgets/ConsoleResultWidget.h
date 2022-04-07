#pragma once
#include <JuceHeader.h>

class ConsoleResultWidget :
    public juce::Component
{
public:
	ConsoleResultWidget();
	~ConsoleResultWidget();

	void init();
	
private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ConsoleResultWidget)
};

