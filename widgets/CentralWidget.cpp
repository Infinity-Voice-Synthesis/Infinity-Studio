#include "CentralWidget.h"
#include "utils/Size.h"

CentralWidget::CentralWidget()
{
	this->menuBar = std::make_unique<MenuBar>();
	this->addChildComponent(this->menuBar.get());
	this->menuBar->setVisible(true);
}

void CentralWidget::init()
{
	this->menuBar->init();
}

void CentralWidget::resized()
{
	const juce::Rectangle<int>& screenArea = Device::getScreenSize();
	const double& screenScale = Device::getScreenScale();

	this->menuBar->setSize(this->getWidth(), Size::menuBar_height * screenArea.getWidth() * screenScale);
	this->menuBar->setTopLeftPosition(0, 0);
}

void CentralWidget::paint(juce::Graphics& g)
{
}