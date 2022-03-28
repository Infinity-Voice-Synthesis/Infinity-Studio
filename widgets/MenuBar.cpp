#include "MenuBar.h"
#include "menus/utils/MenuManager.h"
#include "utils/CallBackManager.h"

MenuBar::MenuBar() :
	MenuBarComponent()
{
	this->menuBarModel = std::make_unique<MenuBarModel>();
}

MenuBar::~MenuBar()
{
	this->setModel(nullptr);
}

void MenuBar::init()
{
	this->menuBarModel->init();
	this->setModel(this->menuBarModel.get());

	MenuManager::connect(
		"MM_Track_View",
		[](bool actived) {CallBackManager::call<void(bool)>("lambda_EditorWidget_TrackViewActived_bool", std::make_tuple(actived)); }
	);
	MenuManager::connect(
		"MM_Note_View",
		[](bool actived) {CallBackManager::call<void(bool)>("lambda_EditorWidget_NoteViewActived_bool", std::make_tuple(actived)); }
	);
	MenuManager::connect(
		"MM_Parameter_View",
		[](bool actived) {CallBackManager::call<void(bool)>("lambda_EditorWidget_ParameterViewActived_bool", std::make_tuple(actived)); }
	);
}
