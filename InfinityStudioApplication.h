﻿#pragma once

#include <JuceHeader.h>
#include "dialogs/MainWindow.h"
#include "google/protobuf/api.pb.h"
#include "utils/Config.h"
#include "menus/MenuManager.h"

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
        Config::refreshTranslates();//配置初始化

        this->loadFont(Config::tsFull("main", "font"));//载入字体

        MenuManager::init();//初始化弹出菜单

        this->mainWindow = std::make_unique<MainWindow>(getApplicationName());
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
        MenuManager::destory();
        Config::destory();
        this->quit();
    }

    void anotherInstanceStarted(const juce::String& commandLine) override
    {
    }

private:
    std::unique_ptr<MainWindow> mainWindow;

    void loadFont(juce::String&& path)
    {
        juce::File file(path);
        juce::FileInputStream stream(file);

        if (stream.failedToOpen()) {
            return;
        }

        size_t fileSize = stream.getTotalLength();

        std::unique_ptr<char[]> fontData(new char[fileSize]);
        stream.read(fontData.get(), fileSize);

        juce::Typeface::Ptr&& typeface = juce::Typeface::createSystemTypefaceFor(fontData.get(), fileSize);
        juce::LookAndFeel::getDefaultLookAndFeel().setDefaultSansSerifTypeface(typeface);
    }
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(InfinityStudioApplication)
};