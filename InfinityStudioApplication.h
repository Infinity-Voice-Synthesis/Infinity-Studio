#pragma once

#include <JuceHeader.h>
#include "dialogs/MainWindow.h"
#include "google/protobuf/api.pb.h"
#include "utils/Config.h"
#include "utils/Source.h"
#include "menus/utils/MenuManager.h"
#include "utils/TextStateGetter.h"
#include "utils/Utils.h"

class InfinityStudioApplication final :
    public juce::JUCEApplication
{
public:
    InfinityStudioApplication() :JUCEApplication() {};

    const juce::String getApplicationName() override { return ProjectInfo::projectName; };
    const juce::String getApplicationVersion() override { return ProjectInfo::versionString; };
    bool moreThanOneInstanceAllowed() override { return false; };

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

        Source::init();//初始化静态资源管理器

        TextStateGetter::init();//初始化字符状态管理器

        MenuManager::init();//初始化弹出菜单

        this->mainWindow = std::make_unique<MainWindow>(getApplicationName());
        this->mainWindow->init();
        this->mainWindow->setVisible(true);
    };

    void shutdown() override
    {
        this->mainWindow = nullptr;
    };

    void systemRequestedQuit() override
    {
        google::protobuf::ShutdownProtobufLibrary();
        MenuManager::destory();
        TextStateGetter::destory();
        Source::destory();
        Config::destory();
        Utils::destory();
        this->quit();
    };

    void anotherInstanceStarted(const juce::String& commandLine) override
    {
    };

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

        Utils::setTypeFace(juce::Typeface::createSystemTypefaceFor(fontData.get(), fileSize));
        juce::LookAndFeel::getDefaultLookAndFeel().setDefaultSansSerifTypeface(Utils::getTypeFace());
    };
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(InfinityStudioApplication)
};