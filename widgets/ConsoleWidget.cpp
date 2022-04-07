#include "ConsoleWidget.h"
#include "utils/Size.h"
#include "utils/Device.h"

ConsoleWidget::ConsoleWidget()
	:Component()
{
	this->consoleToolBar = std::make_unique<ConsoleToolBar>();
	this->addChildComponent(this->consoleToolBar.get());
	this->consoleToolBar->setVisible(true);

	this->consoleResultWidget = std::make_unique<ConsoleResultWidget>();
	this->addChildComponent(this->consoleResultWidget.get());
	this->consoleResultWidget->setVisible(true);

	this->codeDocument = std::make_unique<juce::CodeDocument>();
	this->codeTokeniser = std::make_unique<juce::LuaTokeniser>();
	
	this->codeEditor = std::make_unique<juce::CodeEditorComponent>(
		*(this->codeDocument),
		this->codeTokeniser.get()
		);
	this->addChildComponent(this->codeEditor.get());
	this->codeEditor->setVisible(true);
	
	this->stretchableManager = std::make_unique<juce::StretchableLayoutManager>();
	this->stretchableBar = std::make_unique<juce::StretchableLayoutResizerBar>(
		this->stretchableManager.get(),
		1,
		true
		);
	this->addChildComponent(this->stretchableBar.get());
	this->stretchableBar->setVisible(true);
	
	this->components = {
		this->consoleResultWidget.get(),
		this->stretchableBar.get(),
		this->codeEditor.get()
	};
	
	this->reStr();
}

ConsoleWidget::~ConsoleWidget()
{

}

void ConsoleWidget::init()
{
	this->consoleToolBar->init();
	this->consoleResultWidget->init();
}

void ConsoleWidget::resized()
{
	const juce::Rectangle<int>& screenArea = Device::getScreenSize();
	const double& screenScale = Device::getScreenScale();
	this->consoleToolBar->setSize(Size::consoleToolBar_width * screenArea.getWidth() * screenScale, this->getHeight());
	this->consoleToolBar->setTopLeftPosition(
		this->getWidth() - this->consoleToolBar->getWidth(), 0
	);

	this->stretchableManager->layOutComponents(
		this->components.data(),
		this->components.size(),
		0, 0,
		this->getWidth() - this->consoleToolBar->getWidth(), this->getHeight(),
		false, true
	);
}

void ConsoleWidget::reStr()
{
	int barWidth = Size::consoleSplitor_width * Device::getScreenSize().getWidth() * Device::getScreenScale();
	this->stretchableManager->setItemLayout(
		0,
		-Size::consoleWidget_min_width, -Size::consoleWidget_max_width,
		-Size::consoleWidget_prefred_width
	);
	this->stretchableManager->setItemLayout(
		2,
		-Size::consoleWidget_min_width, -Size::consoleWidget_max_width,
		-Size::consoleWidget_prefred_width
	);
	this->stretchableManager->setItemLayout(
		1,
		barWidth, barWidth,
		barWidth
	);
}
