#include "ConsoleWidget.h"
#include "utils/Size.h"
#include "utils/Device.h"

ConsoleWidget::ConsoleWidget()
	:Component()
{
	this->consoleToolBar = std::make_unique<ConsoleToolBar>();
	this->addChildComponent(this->consoleToolBar.get());
	this->consoleToolBar->setVisible(true);
}

ConsoleWidget::~ConsoleWidget()
{

}

void ConsoleWidget::init()
{
	this->consoleToolBar->init();
}

void ConsoleWidget::resized()
{
	const juce::Rectangle<int>& screenArea = Device::getScreenSize();
	const double& screenScale = Device::getScreenScale();
	this->consoleToolBar->setSize(Size::consoleToolBar_width * screenArea.getWidth() * screenScale, this->getHeight());
	this->consoleToolBar->setTopLeftPosition(
		this->getWidth() - this->consoleToolBar->getWidth(), 0
	);
}