#include "EditorWidget.h"
#include "utils/Size.h"
#include "utils/Device.h"
#include "utils/CallBackManager.h"

EditorWidget::EditorWidget()
	:Component()
{
	this->stretchableManager = std::make_unique<juce::StretchableLayoutManager>();
	this->stretchableBar0 = std::make_unique<juce::StretchableLayoutResizerBar>(
		this->stretchableManager.get(),
		1,
		false
		);
	this->stretchableBar1 = std::make_unique<juce::StretchableLayoutResizerBar>(
		this->stretchableManager.get(),
		3,
		false
		);

	this->trackView = std::make_unique<TrackView>();
	this->noteView = std::make_unique<NoteView>();
	this->paramView = std::make_unique<ParamView>();

	this->components = {
		this->trackView.get()
	};

	this->addChildComponent(this->trackView.get());
	this->addChildComponent(this->noteView.get());
	this->addChildComponent(this->paramView.get());

	this->addChildComponent(this->stretchableBar0.get());
	this->addChildComponent(this->stretchableBar1.get());

	this->trackView->setVisible(true);
	this->noteView->setVisible(false);
	this->paramView->setVisible(false);

	this->stretchableBar0->setVisible(false);
	this->stretchableBar1->setVisible(false);

	this->reStre();
}

EditorWidget::~EditorWidget()
{

}

void EditorWidget::init()
{
	CallBackManager::set<void(bool)>(
		"lambda_EditorWidget_TrackViewActived_bool",
		[this](bool actived) {this->active(actived, EditorWidget::ActivedWidget::TrackView); }
	);
	CallBackManager::set<void(bool)>(
		"lambda_EditorWidget_NoteViewActived_bool",
		[this](bool actived) {this->active(actived, EditorWidget::ActivedWidget::NoteView); }
	);
	CallBackManager::set<void(bool)>(
		"lambda_EditorWidget_ParameterViewActived_bool",
		[this](bool actived) {this->active(actived, EditorWidget::ActivedWidget::ParamView); }
	);
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

void EditorWidget::active(bool actived, EditorWidget::ActivedWidget widget)//将可能情况展开
{
	switch (widget)
	{
	case EditorWidget::ActivedWidget::TrackView:
	{
		if (this->trackView->isVisible() != actived) {
			this->trackView->setVisible(actived);
			if (actived) {
				if ((int)this->noteView->isVisible() + (int)this->paramView->isVisible() == 1) {
					this->stretchableBar0->setVisible(true);
				}//1
				else if ((int)this->noteView->isVisible() + (int)this->paramView->isVisible() == 2) {
					this->stretchableBar1->setVisible(true);
				}//2
				else {
				}//0
			}//开
			else {
				if ((int)this->noteView->isVisible() + (int)this->paramView->isVisible() == 2) {
					this->stretchableBar1->setVisible(false);
				}//2
				else if ((int)this->noteView->isVisible() + (int)this->paramView->isVisible() == 1) {
					this->stretchableBar0->setVisible(false);
				}//1
				else {
				}//0
			}//关
			this->reStre();
		}
		break;
	}//轨道面板
	case EditorWidget::ActivedWidget::NoteView:
	{
		if (this->noteView->isVisible() != actived) {
			this->noteView->setVisible(actived);
			if (actived) {
				if ((int)this->trackView->isVisible() + (int)this->paramView->isVisible() == 1) {
					this->stretchableBar0->setVisible(true);
				}//1
				else if ((int)this->trackView->isVisible() + (int)this->paramView->isVisible() == 2) {
					this->stretchableBar1->setVisible(true);
				}//2
				else {
				}//0
			}//开
			else {
				if ((int)this->trackView->isVisible() + (int)this->paramView->isVisible() == 2) {
					this->stretchableBar1->setVisible(false);
				}//2
				else if ((int)this->trackView->isVisible() + (int)this->paramView->isVisible() == 1) {
					this->stretchableBar0->setVisible(false);
				}//1
				else {
				}//0
			}//关
			this->reStre();
		}
		break;
	}//音符面板
	case EditorWidget::ActivedWidget::ParamView:
	{
		if (this->paramView->isVisible() != actived) {
			this->paramView->setVisible(actived);
			if (actived) {
				if ((int)this->trackView->isVisible() + (int)this->noteView->isVisible() == 1) {
					this->stretchableBar0->setVisible(true);
				}//1
				else if ((int)this->trackView->isVisible() + (int)this->noteView->isVisible() == 2) {
					this->stretchableBar1->setVisible(true);
				}//2
				else {
				}//0
			}//开
			else {
				if ((int)this->trackView->isVisible() + (int)this->noteView->isVisible() == 2) {
					this->stretchableBar1->setVisible(false);
				}//2
				else if ((int)this->trackView->isVisible() + (int)this->noteView->isVisible() == 1) {
					this->stretchableBar0->setVisible(false);
				}//1
				else {
				}//0
			}//关
			this->reStre();
		}
		break;
	}//参数面板
	}
}

void EditorWidget::reStre()
{
	std::vector<juce::Component*>& compL=this->components;
	compL.clear();
	
	auto pushF = [&compL](juce::Component* comp)->void {
		if (comp->isVisible()) {
			compL.push_back(comp);
		}
	};

	pushF(this->trackView.get());
	pushF(this->stretchableBar0.get());
	pushF(this->noteView.get());
	pushF(this->stretchableBar1.get());
	pushF(this->paramView.get());

	const char* barTypeName = "class juce::StretchableLayoutResizerBar";

	if (
		compL.size() == 3 &&
		!strcmp(typeid(*compL.front()).name(), barTypeName)
		) {
		std::iter_swap(compL.begin(), compL.begin() + 1);
	}

	int barHeight = Size::editorSplitor_height * Device::getScreenSize().getHeight() * Device::getScreenScale();

	for (int i = 0; i < compL.size(); i++) {
		juce::Component* comp = compL.at(i);
		const char* id = typeid(*comp).name();

		if (!strcmp(typeid(*comp).name(), barTypeName)) {
			this->stretchableManager->setItemLayout(
				i,
				barHeight, barHeight,
				barHeight
			);
		}
		else {
			this->stretchableManager->setItemLayout(
				i,
				-Size::editorItem_min_height, -Size::editorItem_max_height,
				-Size::editorItem_prefred_height
			);
		}
	}

	this->stretchableManager->layOutComponents(
		this->components.data(),
		this->components.size(),
		0, 0,
		this->getWidth(), this->getHeight(),
		true, true
	);
}