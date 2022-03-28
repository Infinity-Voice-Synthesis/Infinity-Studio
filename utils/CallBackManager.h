#pragma once
#include <JuceHeader.h>

class CallBackManager final
{
	static std::unique_ptr<CallBackManager> _callBackManager;

	class CallBackObjectBase
	{
	};

	template<typename R, typename ...A>
	class CallBackObject final : public CallBackObjectBase
	{
	public:
		explicit CallBackObject() {};

	private:
		JUCE_LEAK_DETECTOR(CallBackObject)
	};

	template<typename R, typename ...A>
	class CallBackObject<std::function<R(A...)>> final :
		public CallBackObjectBase
	{
		using F = std::function<R(A...)>;
		F _data;
	public:
		explicit CallBackObject(const F& data)
			:_data(data)
		{};

		R operator()(std::tuple<A...> args)
		{
			const size_t size = std::tuple_size<typename std::decay<decltype(args)>::type>::value;
			return std::bind(this->_data, std::get<std::make_index_sequence<size>()>(args)...)();
		};

		using ArgType = std::tuple<A...>;
		using RetType = R;
	private:
		JUCE_LEAK_DETECTOR(CallBackObject)
	};

	template<typename ...A>
	class CallBackObject<std::function<void(A...)>> final :
		public CallBackObjectBase
	{
		using F = std::function<void(A...)>;
		F _data;
	public:
		explicit CallBackObject(const F& data)
			:_data(data)
		{};

		void operator()(std::tuple<A...> args)
		{
			const size_t size = std::tuple_size<typename std::decay<decltype(args)>::type>::value;
			std::bind(this->_data, std::get<std::make_index_sequence<size>()>(args)...)();
		};

		using ArgType = std::tuple<A...>;
		using RetType = void;
	private:
		JUCE_LEAK_DETECTOR(CallBackObject)
	};

	using _COB = CallBackObjectBase;
public:
	CallBackManager();
	~CallBackManager();

	static void init();
	static void destory();

	template<typename T, class F = std::function<T>>
	static void set(const juce::String& key, const F& Func)
	{
		using U = CallBackManager::CallBackObject<F>;
		std::unique_ptr<U> pObj = std::make_unique<U>(Func);
		CallBackManager::_callBackManager->list.set(
			key,
			static_cast<_COB>(*pObj)
		);
	};

	template<typename T, class U = CallBackObject<std::function<T>>, typename R = U::RetType, typename A = U::ArgType>
	static R call(const juce::String& key, const A& args)
	{
		if (CallBackManager::_callBackManager->list.contains(key)) {
			U&& obj = static_cast<U&&>(CallBackManager::_callBackManager->list.getReference(key));
			return obj(std::forward<decltype(args)>(args));
		}
	};

	template<typename T, class U = CallBackObject<std::function<T>>, typename R = U::RetType, typename A = U::ArgType>
	requires std::is_void_v<R>
	static R call(const juce::String& key, const A& args)
	{
		if (CallBackManager::_callBackManager->list.contains(key)) {
			U&& obj = static_cast<U&&>(CallBackManager::_callBackManager->list.getReference(key));
			obj(std::forward<decltype(args)>(args));
		}
	};
private:
	juce::HashMap<juce::String, _COB> list;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CallBackManager)
};
