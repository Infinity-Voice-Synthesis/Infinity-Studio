#pragma once
#include <JuceHeader.h>

class ConsoleListModel final :
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
	
	void init(std::function<void(juce::StringRef, MessageType)> clickFunc);

	void addMessage(juce::StringRef message, MessageType type);
	void clear();

	int getNumRows()override;
	void paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected)override;
	void selectedRowsChanged(int lastRowSelected)override;
	juce::String getTooltipForRow(int rowNumber)override;
	
private:
	std::vector<std::pair<juce::String, MessageType>> messageList;
	std::function<void(juce::StringRef, MessageType)> clickFunc;
	
	juce::ReadWriteLock lock;
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ConsoleListModel);
};

