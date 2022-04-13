#pragma once
#include <JuceHeader.h>

class Source
{
	static std::unique_ptr<Source> _source;

public:
	Source();

	static void init();
	static void destory();

	struct Item
	{
		enum class Type {
			Other = 0,
			Image = 1
		};
		Type type = Type::Other;
		std::shared_ptr<char[]> ptr = nullptr;
		size_t size = 0;

	private:
		JUCE_LEAK_DETECTOR(Source::Item);
	};

	static bool get(const juce::String& path, const Item::Type& type, void*& ptr, size_t& size);

	static juce::DrawableImage* makeImage(const juce::String& path);
	static juce::DrawableImage* makeImageSafe(const juce::String& path);

private:
	juce::HashMap<juce::String, Item> list;
	juce::HashMap<juce::String,std::shared_ptr<juce::DrawableImage>> imageList;
};

