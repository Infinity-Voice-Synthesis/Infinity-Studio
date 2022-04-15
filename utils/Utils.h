#pragma once
#include <JuceHeader.h>
#include "TextStateGetter.h"
#include "ImageStateGetter.h"

class Utils final
{
public:
	static void init();
	static void destory();

	static void setTypeFace(juce::Typeface::Ptr typeFace);
	static juce::Typeface::Ptr getTypeFace();

	static TextStateGetter& getTSG();
	static ImageStateGetter& getISG();
private:
	static juce::Typeface::Ptr typeFace;
	static std::unique_ptr<TextStateGetter> tsg;
	static std::unique_ptr<ImageStateGetter> isg;
};
