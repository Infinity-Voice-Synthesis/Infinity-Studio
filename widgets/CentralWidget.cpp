#include "CentralWidget.h"
#include "utils/Size.h"

CentralWidget::CentralWidget()
{
	this->menuBar = std::make_unique<MenuBar>();
	this->addChildComponent(this->menuBar.get());
	this->menuBar->setVisible(true);

	this->toolBar = std::make_unique<ToolBar>();
	this->addChildComponent(this->toolBar.get());
	this->toolBar->setVisible(true);
}

void CentralWidget::init()
{
	this->menuBar->init();
	this->toolBar->init();
}

void CentralWidget::resized()
{
	const juce::Rectangle<int>& screenArea = Device::getScreenSize();
	const double& screenScale = Device::getScreenScale();

	this->menuBar->setSize(this->getWidth(), Size::menuBar_height * screenArea.getHeight() * screenScale);
	this->menuBar->setTopLeftPosition(0, 0);

	this->toolBar->setSize(Size::toolBar_width * screenArea.getWidth() * screenScale, this->getHeight() - this->menuBar->getHeight());
	this->toolBar->setTopLeftPosition(
		(bool)Config::tm("toolBar", "left") ? 0 : this->getWidth() - this->toolBar->getWidth(),
		this->menuBar->getHeight()
	);
}
