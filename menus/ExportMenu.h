#pragma once
#include "menus/utils/Menu.h"

class ExportMenu final :
    public Menu
{
protected:
    const juce::String getMenuId() const override
    {
        return "MM_Export";
    };
    const std::vector<Item> getItemList() const override
    {
        return {
            {Menu::Item::Type::Normal,"MM_To_Midi_File",true,false,"",""},
            {Menu::Item::Type::Normal,"MM_To_Wave_File",true,false,"",""}
        };
    };
};

