#pragma once
#include <JuceHeader.h>

template<class Tx,class Ty>
class StateGetter
{
	static std::unique_ptr<StateGetter<Tx, Ty>> _stateGetter;
public:
	StateGetter();
	~StateGetter();

	static void init();
	static void destory();

	static void insert(const Tx& key, std::function<Ty(const Tx&)> Func);
	static Ty get(const Tx& key);

private:
	juce::HashMap<const Tx&, std::function<Ty(const Tx&)>> list;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StateGetter)
};

template<class Tx, class Ty>
std::unique_ptr<StateGetter<Tx, Ty>> StateGetter<Tx, Ty>::_stateGetter = std::make_unique<StateGetter<Tx, Ty>>();

template<class Tx, class Ty>
void StateGetter<Tx, Ty>::init()
{

}

template<class Tx, class Ty>
void StateGetter<Tx, Ty>::destory()
{
	StateGetter<Tx, Ty>::_stateGetter = nullptr;
}

template<class Tx, class Ty>
StateGetter<Tx, Ty>::StateGetter()
{

}

template<class Tx, class Ty>
StateGetter<Tx, Ty>::~StateGetter()
{

}

template<class Tx, class Ty>
void StateGetter<Tx, Ty>::insert(const Tx& key, std::function<Ty(const Tx&)> Func)
{
	StateGetter<Tx, Ty>::_stateGetter->list.set(key, Func);
}

template<class Tx, class Ty>
Ty StateGetter<Tx, Ty>::get(const Tx& key)
{
	if (!StateGetter<Tx, Ty>::_stateGetter->list.contains(key)) {
		return Ty();
	}
	return StateGetter<Tx, Ty>::_stateGetter->list.getReference(key)(key);
}
