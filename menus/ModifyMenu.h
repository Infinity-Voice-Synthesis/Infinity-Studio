#pragma once
#include "menus/utils/Menu.h"

class ModifyMenu final :
    public Menu
{
public:
    ModifyMenu() : Menu() {};

protected:
    const juce::String getMenuId() const override
    {
        return "MMB_Modify";
    };
    const std::vector<Item> getItemList() const override
    {
        return {
            {Menu::Item::Type::Normal,"MM_Edit_Lyrics",true,false,""},
            {Menu::Item::Type::Separator,"",true,false,""},
            {Menu::Item::Type::Normal,"MM_Modulate",true,false,""},
            {Menu::Item::Type::Normal,"MM_Octave_Up",true,false,""},
            {Menu::Item::Type::Normal,"MM_Octave_Down",true,false,""},
            {Menu::Item::Type::Separator,"",true,false,""},
            {Menu::Item::Type::Normal,"MM_New_Pattern",true,false,""},
            {Menu::Item::Type::Normal,"MM_Delete_Pattern",true,false,""},
            {Menu::Item::Type::Separator,"",true,false,""},
            {Menu::Item::Type::Normal,"MM_New_Parameter_Pattern",true,false,""},
            {Menu::Item::Type::Normal,"MM_Delete_Parameter_Pattern",true,false,""},
            {Menu::Item::Type::Separator,"",true,false,""},
            {Menu::Item::Type::Normal,"MM_Load_Parameter_Pattern",true,false,""},
            {Menu::Item::Type::Normal,"MM_Save_Parameter_Pattern",true,false,""},
            {Menu::Item::Type::Normal,"MM_Manage_Parameter_Pattern",true,false,""}
        };
    };

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModifyMenu)
};

