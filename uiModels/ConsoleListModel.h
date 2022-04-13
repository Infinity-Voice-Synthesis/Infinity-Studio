#pragma once
#include <JuceHeader.h>

class ConsoleListModel :
    public juce::ListBoxModel
{
public:
	ConsoleListModel();
	~ConsoleListModel();
	
	enum class MessageType
	{
		Info,
		Error,
		Input
	};

	void addMessage(const juce::String& message, MessageType type);
	void clear();

	int getNumRows()override;
	void paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected)override;
	void listBoxItemClicked(int row, const juce::MouseEvent& e)override;
	juce::String getTooltipForRow(int rowNumber)override;
	
private:
	std::vector<std::pair<juce::String, MessageType>> messageList;
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ConsoleListModel);
};

