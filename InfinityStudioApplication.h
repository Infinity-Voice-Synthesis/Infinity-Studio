#pragma once

#include <JuceHeader.h>
#include "dialogs/MainWindow.h"
#include "google/protobuf/api.pb.h"
#include "utils/Config.h"
#include "utils/Source.h"
#include "menus/utils/MenuManager.h"
#include "utils/Utils.h"
#include "utils/CallBackManager.h"
#include "Ilvm/ILVM.h"
#include "utils/Egg.h"

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
        Egg::init();//初始化彩蛋
        Utils::init();//初始化单例复用类

        Config::init(
            juce::String("D:/develop/Infinity/"),
            juce::String("main"),
            juce::String("org.infinitysvs.infinitystudio")
        );
        Config::refreshConfigs();
        Config::refreshTranslates();//配置初始化

        ILVM::init(
            [](juce::StringRef str) {CallBackManager::call<void(juce::StringRef)>("lambda_ConsoleWidget_ErrorMessage_const_juce::String&", str); },
            [](juce::StringRef str) {CallBackManager::call<void(juce::StringRef)>("lambda_ConsoleWidget_NormalMessage_const_juce::String&", str); },
            [] {CallBackManager::call<void(void)>("lambda_ConsoleWidget_ClearMessage_void"); },
            [] {CallBackManager::call<void(bool)>("lambda_StatusBar_VMStartStop_bool", true); },
            [] {CallBackManager::call<void(bool)>("lambda_StatusBar_VMStartStop_bool", false); }
        );//初始化Lua虚拟机

        //-------------------------前后端分界-----------------------------//

        this->loadFont(Config::tsFull("main", "font"));//载入字体

        Source::init();//初始化静态资源管理器

        MenuManager::init();//初始化弹出菜单

        CallBackManager::init();//初始化回调管理器

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
        CallBackManager::destory();
        google::protobuf::ShutdownProtobufLibrary();
        MenuManager::destory();
        Source::destory();
        ILVM::destory();
        Config::destory();
        Utils::destory();
        Egg::destory();
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