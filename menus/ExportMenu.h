#pragma once
#include "menus/utils/Menu.h"

class ExportMenu final :
    public Menu
{
public:
    ExportMenu() : Menu() {};

protected:
    const juce::String getMenuId() const override
    {
        return "MM_Export";
    };
    const std::vector<Item> getItemList() const override
    {
        return {
            {Menu::Item::Type::Normal,"MM_To_Midi_File",true,false,""},
            {Menu::Item::Type::Normal,"MM_To_Wave_File",true,false,""}
        };
    };
	
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ExportMenu)
};

