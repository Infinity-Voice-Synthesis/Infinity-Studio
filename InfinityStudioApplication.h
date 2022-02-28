#pragma once

#include <JuceHeader.h>
#include "dialogs/MainWindow.h"
#include "google/protobuf/api.pb.h"
#include "utils/Config.h"

class InfinityStudioApplication : public juce::JUCEApplication
{
public:
    InfinityStudioApplication() {}

    const juce::String getApplicationName() override { return ProjectInfo::projectName; }
    const juce::String getApplicationVersion() override { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override { return false; }

    void initialise(const juce::String& commandLine) override
    {
        Config::init(
            juce::String("D:/develop/Infinity/"),
            juce::String("main"),
            juce::String("org.infinitysvs.infinitystudio")
        );
        Config::refreshConfigs();
        Config::refreshTranslates();

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
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(InfinityStudioApplication)
};