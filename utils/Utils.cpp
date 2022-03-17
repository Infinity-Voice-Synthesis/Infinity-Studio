#include "Utils.h"

juce::Typeface::Ptr Utils::typeFace;
std::unique_ptr<TextStateGetter> Utils::tsg = std::make_unique<TextStateGetter>();
std::unique_ptr<ImageStateGetter> Utils::isg = std::make_unique<ImageStateGetter>();

void Utils::init()
{
}

void Utils::destory()
{
	Utils::typeFace = nullptr;
	Utils::tsg = nullptr;
	Utils::isg = nullptr;
}

void Utils::setTypeFace(juce::Typeface::Ptr typeFace)
{
	Utils::typeFace = typeFace;
}

juce::Typeface::Ptr Utils::getTypeFace()
{
	return Utils::typeFace;
}

TextStateGetter& Utils::getTSG()
{
	return *(Utils::tsg);
}

ImageStateGetter& Utils::getISG()
{
	return *(Utils::isg);
}
