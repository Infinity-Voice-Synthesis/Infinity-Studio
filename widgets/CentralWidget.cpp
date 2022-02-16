#include "CentralWidget.h"
#include "utils/Size.h"

CentralWidget::CentralWidget()
{
	this->menuBar.reset(new MenuBar);
	this->addChildComponent(this->menuBar.get());
	this->menuBar->setVisible(true);
}

void CentralWidget::init()
{
	this->menuBar->init();
}

void CentralWidget::resized()
{
	this->menuBar->setSize(this->getWidth(), Size::menuBar_height * this->getHeight());
	this->menuBar->setTopLeftPosition(0, 0);
}

void CentralWidget::paint(juce::Graphics& g)
{
}