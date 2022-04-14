#include "MenuStateMachine.h"

void MenuStateMachine::add(Menu::Item* item)
{
	if (item == nullptr) {
		return;
	}
	if (
		item->type == Menu::Item::Type::SubMenu ||
		item->type == Menu::Item::Type::Separator ||
		item->type == Menu::Item::Type::Head
		) {
		return;
	}
	if (this->list.contains(item->id)) {
		return;
	}
	this->list.set(item->id, std::make_pair(item, [](bool active) {}));
}

void MenuStateMachine::connect(juce::String id, std::function<void(bool)> Func)
{
	if (!this->list.contains(id)) {
		return;
	}
	this->list.getReference(id).second = Func;
}

void MenuStateMachine::reset(juce::String id)
{
	if (!this->list.contains(id)) {
		return;
	}
	this->list.getReference(id).second = [](bool active) {};
}

void MenuStateMachine::actived(juce::String id)
{
	if (!this->list.contains(id)) {
		return;
	}
	std::pair<Menu::Item*, std::function<void(bool)>>& _value = this->list.getReference(id);
	switch (_value.first->type)
	{
	case Menu::Item::Type::Normal:
	{
		_value.second(true);
		break;
	}
	case Menu::Item::Type::Checkable:
	{
		_value.first->checked = !_value.first->checked;
		this->activeCallBack(_value.first->id, _value.first->checked);
		_value.second(_value.first->checked);
		break;
	}
	case Menu::Item::Type::SingleSelectable:
	{
		bool _breakFlag = false;
		std::function<void(bool)> falseFunc;
		std::function<void(bool)> trueFunc;
		for (auto pair : this->list) {
			if (pair.first->groupId == _value.first->groupId) {
				if (pair.first->id == _value.first->id) {
					if (pair.first->checked) {
						_breakFlag = true;
						break;
					}
					pair.first->checked = true;
					this->activeCallBack(pair.first->id, pair.first->checked);
					trueFunc = pair.second;
				}
				else {
					if (pair.first->checked) {
						pair.first->checked = false;
						this->activeCallBack(pair.first->id, pair.first->checked);
						falseFunc = pair.second;
					}
				}
			}
		}
		if (_breakFlag) {
			break;
		}
		trueFunc(true);
		falseFunc(false);
		break;
	}
	default:
	{
		break;
	}
	}
}

bool MenuStateMachine::isActived(juce::String id)
{
	if (!this->list.contains(id)) {
		return false;
	}
	std::pair<Menu::Item*, std::function<void(bool)>>& _value = this->list.getReference(id);
	return _value.first->checked;
}

void MenuStateMachine::setActiveCallBack(std::function<void(juce::StringRef, bool)> activeCallBack)
{
	this->activeCallBack = activeCallBack;
}