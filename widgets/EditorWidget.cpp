#include "EditorWidget.h"
#include "utils/Size.h"
#include "utils/Device.h"

EditorWidget::EditorWidget()
	:Component()
{
	this->stretchableManager = std::make_unique<juce::StretchableLayoutManager>();
	this->stretchableBar0 = std::make_unique<juce::StretchableLayoutResizerBar>(
		this->stretchableManager.get(),
		this->barId[0],
		false
		);
	this->stretchableBar1 = std::make_unique<juce::StretchableLayoutResizerBar>(
		this->stretchableManager.get(),
		this->barId[1],
		false
		);

	this->trackView = std::make_unique<TrackView>();
	this->noteView = std::make_unique<NoteView>();
	this->paramView = std::make_unique<ParamView>();

	this->components = {
		this->trackView.get(),
		this->stretchableBar0.get(),
		this->noteView.get(),
		this->stretchableBar1.get(),
		this->paramView.get()
	};

	this->addAndMakeVisible(this->trackView.get());
	this->addAndMakeVisible(this->noteView.get());
	this->addAndMakeVisible(this->paramView.get());

	this->addAndMakeVisible(this->stretchableBar0.get());
	this->addAndMakeVisible(this->stretchableBar1.get());

	this->stretchableManager->setItemLayout(
		this->itemId[0],
		-Size::editorItem_min_height, -Size::editorItem_max_height,
		-Size::editorItem_prefred_height
	);
	this->stretchableManager->setItemLayout(
		this->itemId[1],
		-Size::editorItem_min_height, -Size::editorItem_max_height,
		-Size::editorItem_prefred_height
	);
	this->stretchableManager->setItemLayout(
		this->itemId[2],
		-Size::editorItem_min_height, -Size::editorItem_max_height,
		-Size::editorItem_prefred_height
	);

	int barHeight = Size::editorSplitor_height * Device::getScreenSize().getHeight() * Device::getScreenScale();

	this->stretchableManager->setItemLayout(
		this->barId[0],
		barHeight, barHeight,
		barHeight
	);
	this->stretchableManager->setItemLayout(
		this->barId[1],
		barHeight, barHeight,
		barHeight
	);
}

EditorWidget::~EditorWidget()
{

}

void EditorWidget::init()
{
	
}

void EditorWidget::resized()
{
	this->stretchableManager->layOutComponents(
		this->components.data(),
		this->components.size(),
		0, 0,
		this->getWidth(), this->getHeight(),
		true, true
	);
}
