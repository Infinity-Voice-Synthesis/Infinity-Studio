#pragma once

#include <JuceHeader.h>

class Device
{
public:
	static const juce::Rectangle<int> getScreenSize();
	static const double getScreenScale();

private:
	static const juce::Displays::Display* getCurrentScreen();
};

