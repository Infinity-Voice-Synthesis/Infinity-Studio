#include "ConsoleToolBarFactory.h"
#include "utils/CallBackManager.h"

ConsoleToolBarFactory::ConsoleToolBarFactory()
	:ToolBarFactoryBase()
{
    this->toolBarItems = {
        {
            .type = Item::Type::Normal,
            .id = "CTB_RunStop",
            .stateGetter = []()->bool {
            bool isRuning = false;
            CallBackManager::call<void(bool&)>("lambda_StatusBar_VMRunning_bool&",isRuning);
            return isRuning;
            },
            .callBack = [] {
                CallBackManager::call<void(void)>("lambda_ConsoleToolBar_RSActived_void");
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
