#include "NoteView.h"

NoteView::NoteView()
	:Component()
{

}

NoteView::~NoteView()
{

}

void NoteView::paint(juce::Graphics& g)
{
	g.setColour(juce::Colours::darkred);
	g.fillAll();
}
