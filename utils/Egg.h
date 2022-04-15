#pragma once
#include <JuceHeader.h>

class Egg final
{
	static std::unique_ptr<Egg> _egg;
public:
	Egg();
	~Egg();

	static void init();
	static void destory();

	juce::String HB2Myself2022();

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Egg)
};