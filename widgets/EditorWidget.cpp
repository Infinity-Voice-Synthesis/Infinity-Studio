#include "EditorWidget.h"

EditorWidget::EditorWidget()
	:Component()
{
	this->stretchableManager = std::make_unique<juce::StretchableLayoutManager>();
}

EditorWidget::~EditorWidget()
{

}

void EditorWidget::init()
{

}
