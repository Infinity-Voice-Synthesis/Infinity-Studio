#pragma once
#include <JuceHeader.h>
#include "MenuBar.h"
#include "utils/Device.h"
#include "utils/Config.h"
#include "ToolBar.h"
#include "StatusBar.h"
#include "EditorWidget.h"

class CentralWidget final :
    public juce::Component
{
public:
    CentralWidget();

    void init();

    void resized()override;

private:
    std::unique_ptr<MenuBar> menuBar;
    std::unique_ptr<ToolBar> toolBar;
    std::unique_ptr<StatusBar> statusBar;
    std::unique_ptr<EditorWidget> editorWidget;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CentralWidget)
};

