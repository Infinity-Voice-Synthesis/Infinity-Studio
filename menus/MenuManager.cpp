#include "MenuManager.h"

std::unique_ptr<MenuManager> MenuManager::_menuManager = std::make_unique<MenuManager>();
std::unique_ptr<MenuStateMachine> _menuStateMachine = std::make_unique<MenuStateMachine>();

MenuManager::MenuManager()
{

}

void MenuManager::init()
{

}

void MenuManager::destory()
{
	MenuManager::_menuStateMachine = nullptr;
	MenuManager::_menuManager = nullptr;
}

void MenuManager::add(std::shared_ptr<Menu> menu)
{
	MenuManager::_menuManager->list.set(menu->id, menu);
}

juce::PopupMenu MenuManager::get(juce::String id)
{
	if (!MenuManager::_menuManager->list.contains(id)) {
		return juce::PopupMenu();
	}
	return MenuManager::_menuManager->DFS(id);
}

juce::PopupMenu MenuManager::DFS(juce::String id)
{
	Menu& menu = *(this->list.getReference(id));

	juce::PopupMenu pMenu;
	for (int i = 0; i < menu.items.size(); i++) {
		Menu::Item& item = menu.items[i];
		switch (item.type)
		{
		case Menu::Item::Type::Normal:
		{
			pMenu.addItem(
				Config::tr(std::move(item.text)),
				item.enable,
				false,
				[item] {MenuManager::actived(item.text); }
			);
			break;
		}
		case Menu::Item::Type::Checkable:
		{
			pMenu.addItem(
				Config::tr(std::move(item.text)),
				item.enable,
				false,
				[item] {MenuManager::actived(item.text); }
			);
			break;
		}
		case Menu::Item::Type::SingleSelectable:
		{
			pMenu.addItem(
				Config::tr(std::move(item.text)),
				item.enable,
				false,
				[item] {MenuManager::actived(item.text); }
			);
			break;
		}
		case Menu::Item::Type::SubMenu:
		{
			if (!MenuManager::_menuManager->list.contains(item.text)) {
				pMenu.addItem(
					Config::tr(std::move(item.text)),
					item.enable,
					false,
					[] {}
				);
				break;
			}
			pMenu.addSubMenu(
				Config::tr(std::move(item.text)),
				MenuManager::DFS(item.text),
				item.enable
			);
			break;
		}
		case Menu::Item::Type::Separator:
		{
			pMenu.addSeparator();
			break;
		}
		case Menu::Item::Type::Head:
		{
			pMenu.addSectionHeader(Config::tr(std::move(item.text)));
			break;
		}
		}
	}
	return pMenu;
}

void MenuManager::actived(juce::String id)
{
	MenuManager::_menuStateMachine->actived(id);
}
