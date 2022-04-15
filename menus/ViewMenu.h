#pragma once
#include "menus/utils/Menu.h"

class ViewMenu final :
    public Menu
{
public:
    ViewMenu() : Menu() {};

protected:
    const juce::String getMenuId() const override
    {
        return "MMB_View";
    };
    const std::vector<Item> getItemList() const override
    {
        return {
            {Menu::Item::Type::SubMenu,"MM_Snap",true,false,""},
            {Menu::Item::Type::Separator,"",true,false,""},
            {Menu::Item::Type::SingleSelectable,"MM_Explore_Mode",true,true,"MM_View_Mode_Group"},
            {Menu::Item::Type::SingleSelectable,"MM_Edit_Mode",true,false,"MM_View_Mode_Group"},
            {Menu::Item::Type::Separator,"",true,false,""},
            {Menu::Item::Type::Checkable,"MM_Track_View",true,true,""},
            {Menu::Item::Type::Checkable,"MM_Note_View",true,false,""},
            {Menu::Item::Type::Checkable,"MM_Parameter_View",true,false,""},
            {Menu::Item::Type::Separator,"",true,false,""},
            {Menu::Item::Type::SingleSelectable,"MM_Show_Notes",true,true,"MM_Edit_Mode_Group"},
            {Menu::Item::Type::SingleSelectable,"MM_Show_Notes_and_Wave",true,false,"MM_Edit_Mode_Group"},
            {Menu::Item::Type::SingleSelectable,"MM_Show_Notes_and_Pitch_Line",true,false,"MM_Edit_Mode_Group"},
            {Menu::Item::Type::Separator,"",true,false,""},
            {Menu::Item::Type::Checkable,"MM_Show_Library_Image",true,true,""},
            {Menu::Item::Type::Checkable,"MM_Show_Other_Tracks",true,true,""}
        };
    };

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ViewMenu)
};

