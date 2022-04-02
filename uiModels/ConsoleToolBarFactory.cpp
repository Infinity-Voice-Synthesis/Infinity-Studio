#include "ConsoleToolBarFactory.h"
#include "utils/CallBackManager.h"

ConsoleToolBarFactory::ConsoleToolBarFactory()
	:ToolBarFactoryBase()
{
    juce::String id0 = "CTB_RunStop";
    this->toolBarItems = {
        {
            .type = Item::Type::Normal,
            .id = id0,
            .stateGetter = []()->bool {
            bool isRuning = false;
            CallBackManager::call<void(bool&)>("lambda_StatusBar_VMRunning_bool&",isRuning);
            return isRuning;
            },
            .callBack = [id0] {
                CallBackManager::call<void(const juce::String&)>("lambda_ConsoleToolBar_RSActived_const_juce::String&", id0);
            }
        },
        {.type = Item::Type::Normal,.id = "CTB_Load",.toggleAble = false,.callBack = [] {CallBackManager::call<void(void)>("lambda_CodeWidget_Load_void"); }},
        {.type = Item::Type::Normal,.id = "CTB_Save",.toggleAble = false,.callBack = [] {CallBackManager::call<void(void)>("lambda_CodeWidget_Save_void"); }},
        {.type = Item::Type::Normal,.id = "CTB_Exit",.toggleAble = false,.callBack = [] {CallBackManager::call<void(void)>("lambda_CentralWidget_ConsoleExit_void"); }}
    };
}

ConsoleToolBarFactory::~ConsoleToolBarFactory()
{

}
