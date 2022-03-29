#include "ParamView.h"
#include "utils/Size.h"
#include "utils/Device.h"

ParamView::ParamView()
	:Component()
{

}

ParamView::~ParamView()
{

}

void ParamView::paint(juce::Graphics& g)
{
	const juce::Rectangle<int>& screenSize = Device::getScreenSize();
	const int& screenWidth = screenSize.getWidth();
	const int& screenHeight = screenSize.getHeight();
	const double& screenScale = Device::getScreenScale();

	const int& widgetWidth = this->getWidth();
	const int& widgetHeight = this->getHeight();

	double lp = Size::editorPiano_width * screenWidth * screenScale;
	double tp = Size::editorRuler_height * screenHeight * screenScale;
	double rp = widgetWidth - Size::editorVScoller_width * screenWidth * screenScale;
	double bp = widgetHeight - Size::editorHScoller_height * screenHeight * screenScale;

	g.setColour(juce::Colours::white);
	g.drawLine(lp, 0, lp, widgetHeight);
	g.drawLine(rp, 0, rp, widgetHeight);
	if (this->focusOnMode) {
		g.drawLine(0, tp, widgetWidth, tp);
		g.drawLine(0, bp, widgetWidth, bp);
	}
}

void ParamView::setFocusOn(bool focusOn)
{
	if (this->focusOnMode != focusOn) {
		this->focusOnMode = focusOn;
		this->repaint();
	}
}