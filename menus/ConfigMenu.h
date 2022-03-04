#pragma once
#include "menus/utils/Menu.h"

class ConfigMenu final :
    public Menu
{
protected:
    const juce::String getMenuId() const override
    {
        return "MMB_Config";
    };
    const std::vector<Item> getItemList() const override
    {
        return {
            {Menu::Item::Type::Normal,"MM_Editor",true,false,""},
            {Menu::Item::Type::Normal,"MM_Interface",true,false,""},
            {Menu::Item::Type::Normal,"MM_Synthesis",true,false,""},
            {Menu::Item::Type::Normal,"MM_Hardware",true,false,""},
            {Menu::Item::Type::Normal,"MM_Script",true,false,""},
            {Menu::Item::Type::Normal,"MM_Shortcut",true,false,""},
            {Menu::Item::Type::Normal,"MM_Module",true,false,""},
            {Menu::Item::Type::Normal,"MM_Account",true,false,""}
        };
    };
};

