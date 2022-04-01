#include "ToolBarFactory.h"
#include "menus/utils/MenuManager.h"

ToolBarFactory::ToolBarFactory()
    :ToolBarFactoryBase()
{
    auto makeItem = [](const juce::String& id)->const Item {return { .type = Item::Type::Normal, .id = id, .stateGetter = [id]() {return MenuManager::isActived(id); },.callBack = [id] {MenuManager::actived(id); } }; };
    this->toolBarItems = {
        makeItem("MM_Explore_Mode"),
        makeItem("MM_Edit_Mode"),
        {.type = Item::Type::Spacer,.id = ""},
        makeItem("MM_Track_View"),
        makeItem("MM_Note_View"),
        makeItem("MM_Parameter_View"),
        {.type = Item::Type::Spacer,.id = ""},
        makeItem("MM_Show_Notes"),
        makeItem("MM_Show_Notes_and_Wave"),
        makeItem("MM_Show_Notes_and_Pitch_Line"),
        {.type = Item::Type::FlexibleSpacer,.id = ""},
        makeItem("MM_Follow_the_Pointer"),
        makeItem("MM_Loop"),
        makeItem("MM_Play_or_Stop"),
        {.type = Item::Type::Spacer,.id = ""}
    };
}

ToolBarFactory::~ToolBarFactory()
{

}
