#include "ConsoleListModel.h"
#include "utils/Size.h"
#include "utils/Config.h"
#include "utils/Device.h"
#include "utils/Source.h"
#include "utils/Utils.h"

ConsoleListModel::ConsoleListModel()
	:ListBoxModel()
{
	/*this->addMessage("test1", ConsoleListModel::MessageType::Info);
	this->addMessage("test2", ConsoleListModel::MessageType::Info);
	this->addMessage("test3", ConsoleListModel::MessageType::Input);
	this->addMessage("test4", ConsoleListModel::MessageType::Info);
	this->addMessage("test5", ConsoleListModel::MessageType::Error);
	this->addMessage("test6", ConsoleListModel::MessageType::Input);
	this->addMessage("test12345678521234532345654567234545346edr4hrdf5eytr", ConsoleListModel::MessageType::Input);
	this->addMessage("test345tt6y", ConsoleListModel::MessageType::Error);
	this->addMessage("test35trwe5t4wetgqe3dgnuesgnbuedrhnged4iroghbyjewsiotgjia3ejmfriowshtgniweshgyuoer5oriywjse4itgwsi4rhygediojuwosayghiedrfnhujhndrutyhw4uthgoeird5i7y", ConsoleListModel::MessageType::Input);*/
}

ConsoleListModel::~ConsoleListModel()
{
	
}

void ConsoleListModel::init(std::function<void(juce::StringRef, MessageType)> clickFunc)
{
	this->clickFunc = clickFunc;
}

void ConsoleListModel::addMessage(juce::StringRef message, ConsoleListModel::MessageType type)
{
	this->lock.enterWrite();
	this->messageList.push_back(std::make_pair(message, type));
	this->lock.exitWrite();
}

void ConsoleListModel::clear()
{
	this->lock.enterWrite();
	this->messageList.clear();
	this->lock.exitWrite();
}

int ConsoleListModel::getNumRows()
{
	this->lock.enterRead();
	int size = this->messageList.size();
	this->lock.exitRead();
	return size;
}

void ConsoleListModel::paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected)
{
	this->lock.enterRead();
	if (!(rowNumber >= 0 && rowNumber < this->messageList.size())) {
		this->lock.exitRead();
		return;
	}
	const juce::Rectangle<int>& screenArea = Device::getScreenSize();
	const double& screenScale = Device::getScreenScale();
	const int tipWidth = Size::consoleListTip_width * screenArea.getWidth() * screenScale;
	
	auto& pair = this->messageList[this->messageList.size() - rowNumber - 1];
	
	switch (pair.second) {
	case ConsoleListModel::MessageType::Info:
	{
		g.setColour(Config::tc("console", "tip-info-background"));
		g.fillRect(0, 0, width, height);
		g.setColour(Config::tc("console", "tip-info"));
		break;
	}
	case ConsoleListModel::MessageType::Error:
	{
		g.setColour(Config::tc("console", "tip-error-background"));
		g.fillRect(0, 0, width, height);
		g.setColour(Config::tc("console", "tip-error"));
		break;
	}
	case ConsoleListModel::MessageType::Input:
	{
		g.setColour(Config::tc("console", "tip-input-background"));
		g.fillRect(0, 0, width, height);
		g.setColour(Config::tc("console", "tip-input"));
		break;
	}
	}
	if (rowIsSelected) {
		g.fillRect(1, 0, tipWidth, height);
	}
	else {
		g.fillRect(0, 0, tipWidth, height);
	}

	g.setColour(juce::LookAndFeel::getDefaultLookAndFeel().findColour(juce::ListBox::ColourIds::textColourId));
	if (rowIsSelected) {
		g.drawRect(0, 0, width, height);
		juce::DrawableImage* dImage = Source::makeImageSafe(Config::tsFull("LI_Console", "icon-selected"));
		dImage->drawWithin(g, juce::Rectangle<float>(tipWidth, 0, height, height), juce::RectanglePlacement::centred, 1.0);
	}
	
	juce::Font font(Utils::getTypeFace());
	font.setHeight(height);
	g.setFont(font);

	juce::String idText = "#" + juce::String(this->getNumRows() - rowNumber);
	int idWidth = std::min(font.getStringWidthFloat(idText) + height, (width - tipWidth - height) / 2.f);
	g.drawText(idText, tipWidth + height, 0, idWidth, height, juce::Justification::centred);

	g.drawText(
		pair.first.replace("\r\n", "\n").replaceCharacter('\n', ' ').replaceCharacter('\r', ' '),
		tipWidth + height + idWidth, 0,
		width - tipWidth - height - idWidth, height,
		juce::Justification::centredLeft
	);
	
	this->lock.exitRead();
}

void ConsoleListModel::selectedRowsChanged(int lastRowSelected)
{
	this->lock.enterRead();
	if (lastRowSelected >= 0 && lastRowSelected < this->messageList.size()) {
		auto& pair = this->messageList[this->messageList.size() - lastRowSelected - 1];
		this->clickFunc(pair.first, pair.second);
	}
	this->lock.exitRead();
}

juce::String ConsoleListModel::getTooltipForRow(int rowNumber)
{
	this->lock.enterRead();
	if (!(rowNumber >= 0 && rowNumber < this->messageList.size())) {
		this->lock.exitRead();
		return "";
	}
	
	juce::String result = this->messageList[this->messageList.size() - rowNumber - 1].first;
	this->lock.exitRead();
	
	return result;
}
