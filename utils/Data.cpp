#include "Data.h"

juce::Colour Data::parseStringToColour(const juce::String& s)
{
	juce::Colour color;
	if (s.startsWithChar('#')) {
		if (!s.getLastCharacters(s.length() - 1).containsOnly("0123456789abcdefABCDEF")) {
			return juce::Colour();
		}
		if (s.length() == 7) {
			uint8_t r = Data::HexToDec(s[1]) * 16 + Data::HexToDec(s[2]);
			uint8_t g = Data::HexToDec(s[3]) * 16 + Data::HexToDec(s[4]);
			uint8_t b = Data::HexToDec(s[5]) * 16 + Data::HexToDec(s[6]);
			return juce::Colour(r, g, b);
		}
		else if (s.length() == 9) {
			uint8_t r = Data::HexToDec(s[1]) * 16 + Data::HexToDec(s[2]);
			uint8_t g = Data::HexToDec(s[3]) * 16 + Data::HexToDec(s[4]);
			uint8_t b = Data::HexToDec(s[5]) * 16 + Data::HexToDec(s[6]);
			uint8_t a = Data::HexToDec(s[7]) * 16 + Data::HexToDec(s[8]);
			return juce::Colour(r, g, b, a);
		}
		else {
			return juce::Colour();
		}
	}
	else {
		if (!s.containsOnly("0123456789abcdefABCDEF")) {
			return juce::Colour();
		}
		if (s.length() == 6) {
			uint8_t r = Data::HexToDec(s[0]) * 16 + Data::HexToDec(s[1]);
			uint8_t g = Data::HexToDec(s[2]) * 16 + Data::HexToDec(s[3]);
			uint8_t b = Data::HexToDec(s[4]) * 16 + Data::HexToDec(s[5]);
			return juce::Colour(r, g, b);
		}
		else if (s.length() == 8) {
			uint8_t r = Data::HexToDec(s[0]) * 16 + Data::HexToDec(s[1]);
			uint8_t g = Data::HexToDec(s[2]) * 16 + Data::HexToDec(s[3]);
			uint8_t b = Data::HexToDec(s[4]) * 16 + Data::HexToDec(s[5]);
			uint8_t a = Data::HexToDec(s[6]) * 16 + Data::HexToDec(s[7]);
			return juce::Colour(r, g, b, a);
		}
		else {
			return juce::Colour();
		}
	}
	return color;
}

uint8_t Data::HexToDec(const juce::juce_wchar& s)
{
	if (s >= '0' && s <= '9') {
		return s - '0';
	}
	else if (s >= 'a' && s <= 'f') {
		return 9 + s - 'a';
	}
	else if (s >= 'A' && s <= 'F') {
		return 9 + s - 'A';
	}
	else {
		return 0;
	}
}
