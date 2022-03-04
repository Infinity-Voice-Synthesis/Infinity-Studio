#pragma once
#include "menus/utils/Menu.h"

class ImportMenu final :
    public Menu
{
protected:
    const juce::String getMenuId() const override
    {
        return "MM_Import";
    };
    const std::vector<Item> getItemList() const override
    {
        return {
            {Menu::Item::Type::Normal,"MM_From_Midi_File",true,false,""},
            {Menu::Item::Type::Normal,"MM_From_Wave_File",true,false,""}
        };
    };
};

