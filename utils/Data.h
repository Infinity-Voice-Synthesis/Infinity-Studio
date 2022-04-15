#pragma once

#include <JuceHeader.h>

class Data final
{
public:
	static juce::Colour parseStringToColour(const juce::String& s);

private:
	static uint8_t HexToDec(const juce::juce_wchar& s);
};

