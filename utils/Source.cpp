#include "Source.h"

std::unique_ptr<Source> Source::_source = std::make_unique<Source>();

Source::Source()
{

}

void Source::init()
{

}

void Source::destory()
{
	Source::_source = nullptr;
}

bool Source::get(const juce::String& path, const Item::Type& type, void*& ptr, size_t& size)
{
	if (Source::_source->list.contains(path)) {
		Item& _item = Source::_source->list.getReference(path);
		if (_item.type != type) {
			ptr = nullptr;
			size = 0;
			return false;
		}
		ptr = _item.ptr.get();
		size = _item.size;
		return true;
	}
	juce::File file(path);
	juce::FileInputStream stream(file);

	if (stream.failedToOpen()) {
		ptr = nullptr;
		size = 0;
		return false;
	}

	size_t fileSize = stream.getTotalLength();

	Item item;
	item.type = type;
	item.ptr = std::make_shared<char[]>(fileSize);
	item.size = fileSize;
	stream.read(item.ptr.get(), static_cast<int>(fileSize));

	ptr = item.ptr.get();
	size = fileSize;

	Source::_source->list.set(
		path,
		item
	);
	return true;
}

const juce::DrawableImage Source::getImage(const juce::String& path)
{
	void* ptr = nullptr;
	size_t size = 0;
	if (!Source::get(path, Item::Type::Image, ptr, size)) {
		return juce::DrawableImage();
	}

	return juce::DrawableImage(juce::ImageFileFormat::loadFrom(ptr, size));
}

std::unique_ptr<juce::DrawableImage> Source::makeImage(const juce::String& path)
{
	void* ptr = nullptr;
	size_t size = 0;
	if (!Source::get(path, Item::Type::Image, ptr, size)) {
		return std::move(std::make_unique<juce::DrawableImage>());
	}

	return std::move(std::make_unique<juce::DrawableImage>(juce::ImageFileFormat::loadFrom(ptr, size)));
}
