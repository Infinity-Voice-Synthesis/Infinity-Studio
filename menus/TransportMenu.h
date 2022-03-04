#pragma once
#include "menus/utils/Menu.h"

class TransportMenu final :
    public Menu
{
protected:
    const juce::String getMenuId() const override
    {
        return "MMB_Transport";
    };
    const std::vector<Item> getItemList() const override
    {
        return {
            {Menu::Item::Type::Normal,"MM_Play_or_Stop",true,false,""},
            {Menu::Item::Type::Separator,"",true,false,""},
            {Menu::Item::Type::Checkable,"MM_Loop",true,false,""},
            {Menu::Item::Type::Checkable,"MM_Back_on_Stop",true,false,""},
            {Menu::Item::Type::Checkable,"MM_Follow_the_Pointer",true,true,""}
        };
    };
};

