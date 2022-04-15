#pragma once
#include "menus/utils/Menu.h"

class FileMenu final :
    public Menu
{
public:
    FileMenu() : Menu() {};

protected:
    const juce::String getMenuId() const override
    {
        return "MMB_File";
    };
    const std::vector<Item> getItemList() const override
    {
        return {
            {Menu::Item::Type::Normal,"MM_New",true,false,""},
            {Menu::Item::Type::Normal,"MM_Open",true,false,""},
            {Menu::Item::Type::Normal,"MM_Save",true,false,""},
            {Menu::Item::Type::Normal,"MM_Save_as",true,false,""},
            {Menu::Item::Type::Separator,"",true,false,""},
            {Menu::Item::Type::SubMenu,"MM_Import",true,false,""},
            {Menu::Item::Type::SubMenu,"MM_Export",true,false,""},
            {Menu::Item::Type::Separator,"",true,false,""},
            {Menu::Item::Type::Normal,"MM_Close_Editor",true,false,""}
        };
    };

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FileMenu)
};

