#pragma once
#include <JuceHeader.h>
#include "uiModels/ConsoleListModel.h"

class ConsoleResultWidget :
    public juce::Component
{
public:
	ConsoleResultWidget();
	~ConsoleResultWidget();

	void init();
	
	void resized()override;
private:
	std::unique_ptr<juce::TextEditor> textEditor;
	std::unique_ptr<juce::ListBox> listBox;

	std::unique_ptr<juce::StretchableLayoutManager> stretchableManager;
	std::unique_ptr<juce::StretchableLayoutResizerBar> stretchableBar;

	std::vector<juce::Component*> components;

	std::unique_ptr<ConsoleListModel> listModel;
	
	void reStr();
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ConsoleResultWidget)
};

