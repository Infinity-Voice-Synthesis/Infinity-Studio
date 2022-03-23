#include "TrackView.h"

TrackView::TrackView()
	:Component()
{

}

TrackView::~TrackView()
{

}

void TrackView::paint(juce::Graphics& g)
{
	g.setColour(juce::Colours::darkblue);
	g.fillAll();
}