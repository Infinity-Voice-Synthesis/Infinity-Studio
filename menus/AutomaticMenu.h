#pragma once
#include "menus/utils/Menu.h"

class AutomaticMenu final :
    public Menu
{
protected:
    const juce::String getMenuId() const override
    {
        return "MMB_Automatic";
    };
    const std::vector<Item> getItemList() const override
    {
        return {
            {Menu::Item::Type::Normal,"MM_Execute_Plugin",true,false,""},
            {Menu::Item::Type::Normal,"MM_Call_Script",true,false,""},
            {Menu::Item::Type::Normal,"MM_Console_Log",true,false,""},
            {Menu::Item::Type::Normal,"MM_Shell_View",true,false,""},
            {Menu::Item::Type::Normal,"MM_Message_Center",true,false,""}
        };
    };
};

