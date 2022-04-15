#pragma once
#include "menus/utils/Menu.h"

class EditMenu final :
    public Menu
{
public:
    EditMenu() : Menu() {};

protected:
    const juce::String getMenuId() const override
    {
        return "MMB_Edit";
    };
    const std::vector<Item> getItemList() const override
    {
        return {
            {Menu::Item::Type::Normal,"MM_Undo",true,false,""},
            {Menu::Item::Type::Normal,"MM_Redo",true,false,""},
            {Menu::Item::Type::Separator,"",true,false,""},
            {Menu::Item::Type::Normal,"MM_Select_All",true,false,""},
            {Menu::Item::Type::Normal,"MM_Delete",true,false,""},
            {Menu::Item::Type::Normal,"MM_Copy",true,false,""},
            {Menu::Item::Type::Normal,"MM_Cut",true,false,""},
            {Menu::Item::Type::Normal,"MM_Paste",true,false,""}
        };
    };
	
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EditMenu)
};

