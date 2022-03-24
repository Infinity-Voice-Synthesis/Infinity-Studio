#pragma once
#include <JuceHeader.h>

class CallBackManager final
{
	static std::unique_ptr<CallBackManager> _callBackManager;

	class CallBackObjectBase
	{
	};

	template<typename T, class F = std::function<T>>
	class CallBackObject final :
		public CallBackObjectBase
	{
		F _data;
	public:
		explicit CallBackObject(const F& data = F())
			:_data(data)
		{};

		const F& operator()()
		{
			return this->_data;
		};

		void operator()(const F& data)
		{
			this->_data = data;
		};

	private:
		JUCE_LEAK_DETECTOR(CallBackObject)
	};

	using _COB = CallBackObjectBase;
public:
	CallBackManager();
	~CallBackManager();

	static void init();
	static void destory();

	template<typename T, class F = std::function<T>, class U = CallBackObject<T>>
	static void set(const juce::String& key, const F& Func);

	template<typename T, class F = std::function<T>, class U = CallBackObject<T>>
	static void call(const juce::String& key);
private:
	juce::HashMap<juce::String, _COB> list;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CallBackManager)
};

template<typename T, class F, class U>
void CallBackManager::set(const juce::String& key, const F& func)
{
	CallBackManager::_callBackManager->list.set(
		key,
		dynamic_cast<_COB>(U(func))
	);
}

template<typename T, class F, class U>
void CallBackManager::call(const juce::String& key)
{
	if (CallBackManager::_callBackManager->list.contains(key)) {
		const U& obj = CallBackManager::_callBackManager->list.getReference(key);
		const F& func = dynamic_cast<U>(obj)();
		func();
	}
}
