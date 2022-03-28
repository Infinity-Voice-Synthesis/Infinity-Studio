#pragma once
#include <JuceHeader.h>

class CallBackManager final
{
	static std::unique_ptr<CallBackManager> _callBackManager;

	class CallBackObjectBase
	{
	};

	template<typename T>
	class CallBackObject final :
		public CallBackObjectBase
	{
		using F = std::function<T>;
		F _data;
	public:
		CallBackObject(const F& data)
			:_data(data)
		{};

		const F& operator()()
		{
			return this->_data;
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

	template<typename T, class F = std::function<T>, class U = CallBackManager::CallBackObject<T>>
	static void set(const juce::String& key, const F& Func)
	{
		CallBackManager::_callBackManager->list.set(
			key,
			static_cast<_COB>(U(Func))
		);
	};

	template<typename T, typename ...A, class F = std::function<T>, class U = CallBackManager::CallBackObject<T>>
	static void call(const juce::String& key, A ...args)
	{
		if (CallBackManager::_callBackManager->list.contains(key)) {
			U&& obj = static_cast<U&&>(CallBackManager::_callBackManager->list.getReference(key));
			std::bind(obj(), args...)();
		}
	};
private:
	juce::HashMap<juce::String, _COB> list;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CallBackManager)
};
