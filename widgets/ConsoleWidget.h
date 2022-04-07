#pragma once
#include <JuceHeader.h>
#include "ConsoleToolBar.h"
#include "ConsoleResultWidget.h"

class ConsoleWidget :
    public juce::Component
{
public:
    ConsoleWidget();
    ~ConsoleWidget();

    void init();

    void resized()override;

private:
    std::unique_ptr<juce::CodeDocument> codeDocument;
	std::unique_ptr<juce::LuaTokeniser> codeTokeniser;
	
    std::unique_ptr<ConsoleResultWidget> consoleResultWidget;
    std::unique_ptr<juce::CodeEditorComponent> codeEditor;

    std::unique_ptr<juce::StretchableLayoutManager> stretchableManager;
	std::unique_ptr<juce::StretchableLayoutResizerBar> stretchableBar;
	
    std::unique_ptr<ConsoleToolBar> consoleToolBar;
	
    std::vector<juce::Component*> components;

    void reStr();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ConsoleWidget)
};

