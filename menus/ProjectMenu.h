#pragma once
#include "menus/utils/Menu.h"

class ProjectMenu final :
    public Menu
{
public:
    ProjectMenu() : Menu() {};

protected:
    const juce::String getMenuId() const override
    {
        return "MMB_Project";
    };
    const std::vector<Item> getItemList() const override
    {
        return {
            {Menu::Item::Type::Normal,"MM_Beat",true,false,""},
            {Menu::Item::Type::Normal,"MM_Tempo",true,false,""},
            {Menu::Item::Type::Separator,"",true,false,""},
            {Menu::Item::Type::Normal,"MM_Track_Mute",true,false,""},
            {Menu::Item::Type::Normal,"MM_Track_Solo",true,false,""},
            {Menu::Item::Type::Separator,"",true,false,""},
            {Menu::Item::Type::Normal,"MM_New_Track",true,false,""},
            {Menu::Item::Type::Normal,"MM_Delete_Track",true,false,""},
            {Menu::Item::Type::Normal,"MM_Last_Track",true,false,""},
            {Menu::Item::Type::Normal,"MM_Next_Track",true,false,""},
            {Menu::Item::Type::Separator,"",true,false,""},
            {Menu::Item::Type::Normal,"MM_Voice_Library",true,false,""}
        };
    };

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProjectMenu)
};

