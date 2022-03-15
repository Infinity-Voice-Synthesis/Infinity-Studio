#include "Utils.h"

juce::Typeface::Ptr Utils::typeFace;

void Utils::destory()
{
	Utils::typeFace = nullptr;
}

void Utils::setTypeFace(juce::Typeface::Ptr typeFace)
{
	Utils::typeFace = typeFace;
}

juce::Typeface::Ptr Utils::getTypeFace()
{
	return Utils::typeFace;
}