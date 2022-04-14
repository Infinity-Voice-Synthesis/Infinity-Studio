#include "ConsoleWidget.h"
#include "utils/Size.h"
#include "utils/Device.h"
#include "utils/Config.h"
#include "utils/CallBackManager.h"
#include "Ilvm/ILVM.h"

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
	
	this->fileChooser = std::make_unique<juce::FileChooser>(
		Config::tr("CFD_TitleLoad"),
		Config::scriptDir(), "*.lua",
		true, true,
		this
		);
	this->saveFileChooser = std::make_unique<juce::FileChooser>(
		Config::tr("CFD_TitleSave"),
		Config::scriptDir(), "*.lua",
		true, true,
		this
		);
}

ConsoleWidget::~ConsoleWidget()
{

}

void ConsoleWidget::init()
{
	this->consoleToolBar->init([this] {this->codeRunStop(); });

	juce::CodeDocument* codeDocument = this->codeDocument.get();
	auto funcCodeChange = [codeDocument](juce::StringRef message)
	{
		codeDocument->replaceAllContent(message);
	};
	this->consoleResultWidget->init(funcCodeChange);

	juce::CodeEditorComponent::ColourScheme cs;
	
	auto csF = [&cs](juce::StringRef id) {
		cs.set(id, Config::tc("codeEditor", id));
	};
	
	csF("Error");
	csF("Comment");
	csF("Keyword");
	csF("Operator");
	csF("Identifier");
	csF("Integer");
	csF("Float");
	csF("String");
	csF("Bracket");
	csF("Punctuation");
	csF("Preprocessor Text");
	
	this->codeEditor->setColourScheme(cs);
	
	CallBackManager::set<void(void)>("lambda_CodeWidget_Load_void", [this] {this->load(); });
	CallBackManager::set<void(void)>("lambda_CodeWidget_Save_void", [this] {this->save(); });
	
	CallBackManager::set<void(juce::StringRef)>(
		"lambda_ConsoleWidget_ErrorMessage_const_juce::String&",
		[this](juce::StringRef mes) {this->errorMessage(mes); }
	);
	CallBackManager::set<void(juce::StringRef)>(
		"lambda_ConsoleWidget_NormalMessage_const_juce::String&",
		[this](juce::StringRef mes) {this->normalMessage(mes); }
	);
	CallBackManager::set<void(void)>("lambda_ConsoleWidget_ClearMessage_void", [this] {this->clearMessage(); });
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

	this->codeEditor->setFont(this->codeEditor->getFont().withHeight(Size::consoleFont_height * screenArea.getHeight() * screenScale));
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

void ConsoleWidget::load()
{
	if (this->fileChooser->showDialog(
		juce::FileBrowserComponent::FileChooserFlags::canSelectFiles |
		juce::FileBrowserComponent::FileChooserFlags::openMode,
		nullptr
	)) {
		juce::File file = this->fileChooser->getResult();
		juce::MemoryBlock memdata;
		file.loadFileAsData(memdata);
		this->codeDocument->replaceAllContent(memdata.toString());
	}
}

void ConsoleWidget::save()
{
	if (this->saveFileChooser->showDialog(
		juce::FileBrowserComponent::FileChooserFlags::canSelectFiles |
		juce::FileBrowserComponent::FileChooserFlags::saveMode |
		juce::FileBrowserComponent::FileChooserFlags::warnAboutOverwriting,
		nullptr
	)) {
		juce::File file = this->saveFileChooser->getResult();
		juce::String fileStr = this->codeDocument->getAllContent();
		const char* content = fileStr.toRawUTF8();
		int size = fileStr.length();
		if (file.replaceWithData(content, size)) {
			juce::AlertWindow::showMessageBox(
				juce::AlertWindow::AlertIconType::InfoIcon,
				Config::tr("CFD_TitleSave"),
				Config::tr("CFD_MessageSaveOK_0") +
				file.getFullPathName() +
				Config::tr("CFD_MessageSaveOK_1")
			);
		}
		else {
			juce::AlertWindow::showMessageBox(
				juce::AlertWindow::AlertIconType::WarningIcon,
				Config::tr("CFD_TitleSave"),
				Config::tr("CFD_MessageSaveError_0") +
				file.getFullPathName() +
				Config::tr("CFD_MessageSaveError_1")
			);
		}
	}
}

void ConsoleWidget::codeRunStop()
{
	bool isRuning = false;
	CallBackManager::call<void(bool*)>("lambda_StatusBar_VMRunning_bool&", &isRuning);
	if (isRuning) {
		ILVM::stop();
	}
	else {
		juce::String code = this->codeDocument->getAllContent();
		this->consoleResultWidget->addMessage(code, ConsoleListModel::MessageType::Input);
		ILVM::run(code);
	}
}

void ConsoleWidget::errorMessage(juce::StringRef message)
{
	this->consoleResultWidget->addMessage(message, ConsoleListModel::MessageType::Error);
}

void ConsoleWidget::normalMessage(juce::StringRef message)
{
	this->consoleResultWidget->addMessage(message, ConsoleListModel::MessageType::Info);
}

void ConsoleWidget::clearMessage()
{
	this->consoleResultWidget->clear();
}
