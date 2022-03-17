#pragma once
#include <JuceHeader.h>

template<class Tx,class Ty,class F=std::function<Ty(const Tx&)>>
class StateGetter
{
public:
	StateGetter() {};
	~StateGetter() {};

	void insert(const Tx& key, F Func)
	{
		this->funcList->set(key, Func);
	};
	const Ty get(const Tx& key)
	{
		if (!this->funcList->contains(key)) {
			return *(this->_empty);
		}
		return (this->funcList->getReference(key))(key);
	};

private:
	using TMap = juce::HashMap<Tx, F>;
	std::unique_ptr<TMap> funcList
		= std::make_unique<TMap>();
	std::unique_ptr<Ty> _empty
		= std::make_unique<Ty>();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StateGetter)
};
