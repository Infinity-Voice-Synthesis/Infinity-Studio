#include "ConsoleResultWidget.h"
#include "utils/Size.h"
#include "utils/Device.h"

ConsoleResultWidget::ConsoleResultWidget()
	:Component()
{
	this->textEditor = std::make_unique<juce::TextEditor>();
	this->textEditor->setMultiLine(true);
	this->textEditor->setReadOnly(true);
	this->addChildComponent(this->textEditor.get());
	this->textEditor->setVisible(true);
	
	this->listBox = std::make_unique<juce::ListBox>();
	this->addChildComponent(this->listBox.get());
	this->listBox->setVisible(true);
	
	this->stretchableManager = std::make_unique<juce::StretchableLayoutManager>();
	this->stretchableBar = std::make_unique<juce::StretchableLayoutResizerBar>(
		this->stretchableManager.get(),
		1,
		false
		);
	
	this->addChildComponent(this->stretchableBar.get());
	this->stretchableBar->setVisible(true);
		
	this->components = {
		this->textEditor.get(),
		this->stretchableBar.get(),
		this->listBox.get()
	};

	this->reStr();
}

ConsoleResultWidget::~ConsoleResultWidget()
{
	
}

void ConsoleResultWidget::init()
{
	
}

void ConsoleResultWidget::resized()
{
	const juce::Rectangle<int>& screenArea = Device::getScreenSize();
	const double& screenScale = Device::getScreenScale();
	
	this->stretchableManager->layOutComponents(
		this->components.data(),
		this->components.size(),
		0, 0,
		this->getWidth(), this->getHeight(),
		true, true
	);
	
	this->textEditor->setFont(this->textEditor->getFont().withHeight(Size::consoleFont_height * screenArea.getHeight() * screenScale));
}

void ConsoleResultWidget::reStr()
{
	int barHeight = Size::consoleSplitor_height * Device::getScreenSize().getHeight() * Device::getScreenScale();
	this->stretchableManager->setItemLayout(
		0,
		-Size::consoleWidget_min_height, -Size::consoleWidget_max_height,
		-Size::consoleWidget_prefred_height
	);
	this->stretchableManager->setItemLayout(
		2,
		-Size::consoleWidget_min_height, -Size::consoleWidget_max_height,
		-Size::consoleWidget_prefred_height
	);
	this->stretchableManager->setItemLayout(
		1,
		barHeight, barHeight,
		barHeight
	);
}