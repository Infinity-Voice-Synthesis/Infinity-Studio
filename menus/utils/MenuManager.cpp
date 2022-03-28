#include "MenuManager.h"
#include "menus/Menus.h"

std::unique_ptr<MenuManager> MenuManager::_menuManager = std::make_unique<MenuManager>();
std::unique_ptr<MenuStateMachine> MenuManager::_menuStateMachine = std::make_unique<MenuStateMachine>();

MenuManager::MenuManager()
{

}

void MenuManager::init()
{
	(new FileMenu)->init();
	(new ImportMenu)->init();
	(new ExportMenu)->init();
	(new EditMenu)->init();
	(new ViewMenu)->init();
	(new SnapMenu)->init();
	(new ModifyMenu)->init();
	(new ProjectMenu)->init();
	(new TransportMenu)->init();
	(new AutomaticMenu)->init();
	(new ConfigMenu)->init();
	(new MiscMenu)->init();
}

void MenuManager::destory()
{
	MenuManager::_menuStateMachine = nullptr;
	MenuManager::_menuManager = nullptr;
}

void MenuManager::add(std::shared_ptr<Menu> menu)
{
	MenuManager::_menuManager->list.set(menu->id, menu);
	for (auto& i : menu->items) {
		MenuManager::_menuStateMachine->add(&i);
	}
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
		Menu::Item& item = menu.items.at(i);
		switch (item.type)
		{
		case Menu::Item::Type::Normal:
		{
			pMenu.addItem(
				Config::tr(item.id),
				item.enable,
				false,
				[item] {MenuManager::actived(item.id); }
			);
			break;
		}
		case Menu::Item::Type::Checkable:
		{
			pMenu.addItem(
				Config::tr(item.id),
				item.enable,
				item.checked,
				[item] {MenuManager::actived(item.id); }
			);
			break;
		}
		case Menu::Item::Type::SingleSelectable:
		{
			pMenu.addItem(
				Config::tr(item.id),
				item.enable,
				item.checked,
				[item] {MenuManager::actived(item.id); }
			);
			break;
		}
		case Menu::Item::Type::SubMenu:
		{
			if (!MenuManager::_menuManager->list.contains(item.id)) {
				pMenu.addItem(
					Config::tr(item.id),
					item.enable,
					false,
					[] {}
				);
				break;
			}
			pMenu.addSubMenu(
				Config::tr(item.id),
				MenuManager::DFS(item.id),
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
			pMenu.addSectionHeader(Config::tr(item.id));
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

bool MenuManager::isActived(juce::String id)
{
	return MenuManager::_menuStateMachine->isActived(id);
}

void MenuManager::setActiveCallBack(std::function<void(const juce::String&, bool)> activeCallBack)
{
	MenuManager::_menuStateMachine->setActiveCallBack(activeCallBack);
}

void MenuManager::connect(juce::String id, std::function<void(bool)> Func)
{
	MenuManager::_menuStateMachine->connect(id, Func);
}

void MenuManager::reset(juce::String id)
{
	MenuManager::_menuStateMachine->reset(id);
}
