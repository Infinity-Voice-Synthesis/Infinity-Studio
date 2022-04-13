#include "ConsoleListModel.h"
#include "utils/Size.h"
#include "utils/Config.h"
#include "utils/Device.h"
#include "utils/Source.h"
#include "utils/Utils.h"

ConsoleListModel::ConsoleListModel()
	:ListBoxModel()
{
	this->addMessage("test1", ConsoleListModel::MessageType::Info);
	this->addMessage("test2", ConsoleListModel::MessageType::Info);
	this->addMessage("test3", ConsoleListModel::MessageType::Input);
	this->addMessage("test4", ConsoleListModel::MessageType::Info);
	this->addMessage("test5", ConsoleListModel::MessageType::Error);
	this->addMessage("test6", ConsoleListModel::MessageType::Input);
	this->addMessage("test12345678521234532345654567234545346edr4hrdf5eytr", ConsoleListModel::MessageType::Input);
	this->addMessage("test345tt6y", ConsoleListModel::MessageType::Error);
	this->addMessage("test35trwe5t4wetgqe3dgnuesgnbuedrhnged4iroghbyjewsiotgjia3ejmfriowshtgniweshgyuoer5oriywjse4itgwsi4rhygediojuwosayghiedrfnhujhndrutyhw4uthgoeird5i7y", ConsoleListModel::MessageType::Input);
}

ConsoleListModel::~ConsoleListModel()
{
	
}

void ConsoleListModel::addMessage(const juce::String& message, ConsoleListModel::MessageType type)
{
	this->messageList.push_back(std::make_pair(message, type));
}

void ConsoleListModel::clear()
{
	this->messageList.clear();
}

int ConsoleListModel::getNumRows()
{
	return this->messageList.size();
}

void ConsoleListModel::paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected)
{
	if (!(rowNumber >= 0 && rowNumber < this->messageList.size())) {
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

	g.drawText(pair.first, tipWidth + height + idWidth, 0, width - tipWidth - height - idWidth, height, juce::Justification::centredLeft);
}

void ConsoleListModel::listBoxItemClicked(int row, const juce::MouseEvent& e)
{
	
}

juce::String ConsoleListModel::getTooltipForRow(int rowNumber)
{
	if (!(rowNumber >= 0 && rowNumber < this->messageList.size())) {
		return "";
	}
	return this->messageList[this->messageList.size() - rowNumber - 1].first;
}
