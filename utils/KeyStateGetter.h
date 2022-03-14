#pragma once
#include "StateGetter.h"

template<class T>
using KeyStateGetter = StateGetter<juce::String, T>;
