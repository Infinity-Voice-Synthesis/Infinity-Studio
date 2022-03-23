#include "ParamView.h"

ParamView::ParamView()
	:Component()
{

}

ParamView::~ParamView()
{

}

void ParamView::paint(juce::Graphics& g)
{
	g.setColour(juce::Colours::darkgreen);
	g.fillAll();
}