#include "Device.h"

const juce::Rectangle<int> Device::getScreenSize()
{
	const juce::Displays::Display* display = Device::getCurrentScreen();
	if (display == nullptr) {
		return juce::Rectangle<int>();
	}
	return display->userArea;
}

const double Device::getScreenScale()
{
	const juce::Displays::Display* display = Device::getCurrentScreen();
	if (display == nullptr) {
		return 1;
	}
	return display->scale;
}

const juce::Displays::Display* Device::getCurrentScreen()
{
	return juce::Desktop::getInstance().getDisplays().getPrimaryDisplay();
}
