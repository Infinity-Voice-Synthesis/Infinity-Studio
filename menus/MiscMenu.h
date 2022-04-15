#pragma once
#include "menus/utils/Menu.h"

class MiscMenu final :
    public Menu
{
public:
    MiscMenu() : Menu() {};
	
protected:
    const juce::String getMenuId() const override
    {
        return "MMB_Misc";
    };
    const std::vector<Item> getItemList() const override
    {
        return {
            {Menu::Item::Type::Normal,"MM_About",true,false,""},
            {Menu::Item::Type::Normal,"MM_License",true,false,""},
            {Menu::Item::Type::Normal,"MM_Website",true,false,""},
            {Menu::Item::Type::Normal,"MM_Develop",true,false,""},
            {Menu::Item::Type::Normal,"MM_Update",true,false,""}
        };
    };

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MiscMenu)
};

