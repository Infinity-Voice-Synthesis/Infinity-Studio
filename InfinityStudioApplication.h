#pragma once

#include <JuceHeader.h>
#include "dialogs/MainWindow.h"
#include "google/protobuf/api.pb.h"

class InfinityStudioApplication : public juce::JUCEApplication
{
public:
    InfinityStudioApplication() {}

    const juce::String getApplicationName() override { return ProjectInfo::projectName; }
    const juce::String getApplicationVersion() override { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override { return false; }

    void initialise(const juce::String& commandLine) override
    {

        this->mainWindow.reset(new MainWindow(getApplicationName()));

        this->mainWindow->init();

        this->mainWindow->setVisible(true);
    }

    void shutdown() override
    {
        this->mainWindow = nullptr;
    }

    void systemRequestedQuit() override
    {
        google::protobuf::ShutdownProtobufLibrary();
        this->quit();
    }

    void anotherInstanceStarted(const juce::String& commandLine) override
    {
    }

private:
    std::unique_ptr<MainWindow> mainWindow;
    
};