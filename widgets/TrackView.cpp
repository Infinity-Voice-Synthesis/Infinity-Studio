#include "TrackView.h"
#include "utils/Size.h"
#include "utils/Device.h"

TrackView::TrackView()
	:Component()
{

}

TrackView::~TrackView()
{

}

void TrackView::paint(juce::Graphics& g)
{
	const juce::Rectangle<int>& screenSize = Device::getScreenSize();
	const int& screenWidth = screenSize.getWidth();
	const int& screenHeight = screenSize.getHeight();
	const double& screenScale = Device::getScreenScale();

	const int& widgetWidth = this->getWidth();
	const int& widgetHeight = this->getHeight();

	double lp = Size::editorTrackHead_width * screenWidth * screenScale;
	double tp = Size::editorRuler_height * screenHeight * screenScale;
	double rp = widgetWidth - Size::editorVScoller_width * screenWidth * screenScale;
	double bp = widgetHeight - Size::editorHScoller_height * screenHeight * screenScale;

	g.setColour(juce::Colours::white);
	g.drawLine(lp, 0, lp, widgetHeight);
	g.drawLine(0, tp, widgetWidth, tp);
	g.drawLine(rp, 0, rp, widgetHeight);
	g.drawLine(0, bp, widgetWidth, bp);
}