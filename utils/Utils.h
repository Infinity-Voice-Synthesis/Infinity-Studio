#pragma once
#include <JuceHeader.h>

class Utils
{
public:
	static void destory();

	static void setTypeFace(juce::Typeface::Ptr typeFace);
	static juce::Typeface::Ptr getTypeFace();
private:
	static juce::Typeface::Ptr typeFace;
};
