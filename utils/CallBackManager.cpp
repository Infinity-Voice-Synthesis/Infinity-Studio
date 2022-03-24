#include "CallBackManager.h"

std::unique_ptr<CallBackManager> CallBackManager::_callBackManager = std::make_unique<CallBackManager>();

CallBackManager::CallBackManager()
{

}

CallBackManager::~CallBackManager()
{

}

void CallBackManager::init()
{

}

void CallBackManager::destory()
{
	CallBackManager::_callBackManager = nullptr;
}
