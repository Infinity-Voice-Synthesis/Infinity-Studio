#pragma once
#include "menus/utils/Menu.h"

class SnapMenu final :
    public Menu
{
public:
    SnapMenu() : Menu() {};

protected:
    const juce::String getMenuId() const override
    {
        return "MM_Snap";
    };
    const std::vector<Item> getItemList() const override
    {
        return {
            {Menu::Item::Type::SingleSelectable,"MM_Snap_to_Beat",true,true,"MM_Snap_Group"},
            {Menu::Item::Type::SingleSelectable,"MM_Snap_to_1_2_Beat",true,false,"MM_Snap_Group"},
            {Menu::Item::Type::SingleSelectable,"MM_Snap_to_1_4_Beat",true,false,"MM_Snap_Group"},
            {Menu::Item::Type::SingleSelectable,"MM_Snap_to_1_6_Beat",true,false,"MM_Snap_Group"},
            {Menu::Item::Type::SingleSelectable,"MM_Snap_to_1_8_Beat",true,false,"MM_Snap_Group"},
            {Menu::Item::Type::SingleSelectable,"MM_Snap_to_1_12_Beat",true,false,"MM_Snap_Group"},
            {Menu::Item::Type::SingleSelectable,"MM_Snap_to_1_16_Beat",true,false,"MM_Snap_Group"},
            {Menu::Item::Type::SingleSelectable,"MM_Snap_to_1_24_Beat",true,false,"MM_Snap_Group"},
            {Menu::Item::Type::SingleSelectable,"MM_Snap_to_1_32_Beat",true,false,"MM_Snap_Group"},
            {Menu::Item::Type::SingleSelectable,"MM_Snap_Off",true,false,"MM_Snap_Group"}
        };
    };

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SnapMenu)
};

