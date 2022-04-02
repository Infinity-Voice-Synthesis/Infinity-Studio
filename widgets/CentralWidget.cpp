#include "CentralWidget.h"
#include "utils/Size.h"
#include "utils/CallBackManager.h"
#include "menus/utils/MenuManager.h"

CentralWidget::CentralWidget() :
	Component()
{
	this->menuBar = std::make_unique<MenuBar>();
	this->addChildComponent(this->menuBar.get());
	this->menuBar->setVisible(true);

	this->toolBar = std::make_unique<ToolBar>();
	this->addChildComponent(this->toolBar.get());
	this->toolBar->setVisible(true);

	this->statusBar = std::make_unique<StatusBar>();
	this->addChildComponent(this->statusBar.get());
	this->statusBar->setVisible(true);

	this->editorWidget = std::make_unique<EditorWidget>();
	this->addChildComponent(this->editorWidget.get());
	this->editorWidget->setVisible(true);

	this->consoleWidget = std::make_unique<ConsoleWidget>();
	this->addChildComponent(this->consoleWidget.get());
	this->consoleWidget->setVisible(false);
}

void CentralWidget::init()
{
	this->menuBar->init();
	this->toolBar->init();
	this->statusBar->init();
	this->editorWidget->init();
	this->consoleWidget->init();

	CallBackManager::set<void(void)>(
		"lambda_CentralWidget_ConsoleChange_void",
		[this] {this->consoleVisibleChange(); }
	);
	CallBackManager::set<void(void)>(
		"lambda_CentralWidget_ConsoleExit_void",
		[this] {this->consoleVisibleChange(); }
	);
	MenuManager::connect(
		"MM_Shell_View",
		[this](bool actived) {this->openConsole(); }
	);
}

void CentralWidget::resized()
{
	const juce::Rectangle<int>& screenArea = Device::getScreenSize();
	const double& screenScale = Device::getScreenScale();

	this->menuBar->setSize(this->getWidth(), Size::menuBar_height * screenArea.getHeight() * screenScale);
	this->menuBar->setTopLeftPosition(0, 0);

	this->statusBar->setSize(this->getWidth(), Size::statusBar_height * screenArea.getHeight() * screenScale);
	this->statusBar->setTopLeftPosition(0, this->getHeight() - this->statusBar->getHeight());

	this->toolBar->setSize(Size::toolBar_width * screenArea.getWidth() * screenScale, this->getHeight() - this->menuBar->getHeight() - this->statusBar->getHeight());
	this->toolBar->setTopLeftPosition(
		(bool)Config::tm("toolBar", "left") ? 0 : this->getWidth() - this->toolBar->getWidth(),
		this->menuBar->getHeight()
	);

	this->editorWidget->setSize(this->getWidth() - this->toolBar->getWidth(), this->getHeight() - this->menuBar->getHeight() - this->statusBar->getHeight());
	this->editorWidget->setTopLeftPosition(
		(bool)Config::tm("toolBar", "left") ? this->toolBar->getWidth() : 0,
		this->menuBar->getHeight()
	);

	this->consoleWidget->setSize(this->getWidth() - this->toolBar->getWidth(), this->getHeight() - this->menuBar->getHeight() - this->statusBar->getHeight());
	this->consoleWidget->setTopLeftPosition(
		(bool)Config::tm("toolBar", "left") ? this->toolBar->getWidth() : 0,
		this->menuBar->getHeight()
	);
}

void CentralWidget::consoleVisibleChange()
{
	bool cV = this->consoleWidget->isVisible();
	this->consoleWidget->setVisible(!cV);
	this->editorWidget->setVisible(cV);
}

void CentralWidget::openConsole()
{
	if (!this->consoleWidget->isVisible()) {
		this->consoleVisibleChange();
	}
}
