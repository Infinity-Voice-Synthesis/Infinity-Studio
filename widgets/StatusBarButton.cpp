#include "StatusBarButton.h"
#include "utils/Utils.h"

StatusBarButton::ItemStateCheckFunction StatusBarButton::stateCheckFunc
= [](const StatusBarButton::ShowType& arg1, const StatusBarButton::ShowType& arg2) ->bool
{
    return static_cast<uint64_t>(arg1) & static_cast<uint64_t>(arg2);
};

StatusBarButton::StatusBarButton(const int iid, ShowType showType, const juce::String& buttonText,
    std::unique_ptr<juce::Drawable> normalIm,
    std::unique_ptr<juce::Drawable> toggledOnIm)
    :ToolbarItemComponent(iid,buttonText,true),
    showType(showType),
    normalImage(std::move(normalIm)),
    toggledOnImage(std::move(toggledOnIm))
{
    jassert(normalImage != nullptr);
}

StatusBarButton::~StatusBarButton()
{

}

bool StatusBarButton::getToolbarItemSizes(int toolbarDepth, bool isToolbarVertical, int& preferredSize, int& minSize, int& maxSize)
{
    if (isToolbarVertical) {
        return false;
    }
    const juce::String& text = this->getButtonText();
    juce::Font font(Utils::getTypeFace());
    font.setHeight(toolbarDepth);
    int stringWidth = font.getStringWidth(text);
    preferredSize = minSize = maxSize
        = (
            (this->stateCheckFunc(this->showType, ShowType::Icon) ? toolbarDepth : 0) +
            (this->stateCheckFunc(this->showType, ShowType::Text) ? stringWidth : 0)
            );
    return true;
}

void StatusBarButton::paintButtonArea(juce::Graphics& graphic, int width, int height, bool /*isMouseOver*/, bool /*isMouseDown*/)
{
    if (!this->stateCheckFunc(this->showType, ShowType::Text)) {
        return;
    }
    juce::Font font(Utils::getTypeFace());
    font.setHeight(height);
    graphic.setFont(font);
    graphic.setColour(juce::LookAndFeel::getDefaultLookAndFeel().findColour(juce::Toolbar::ColourIds::labelTextColourId));
    graphic.drawText(
        this->getButtonText(),
        juce::Rectangle<float>(
            this->stateCheckFunc(this->showType, ShowType::Icon) ? height : 0,
            0,
            this->stateCheckFunc(this->showType, ShowType::Icon) ? (width - height) : width,
            height
            ),
        juce::Justification::centred
    );
}

void StatusBarButton::contentAreaChanged(const juce::Rectangle<int>&)
{
    buttonStateChanged();
}

void StatusBarButton::setCurrentImage(juce::Drawable* const newImage)
{
    if (newImage != currentImage)
    {
        removeChildComponent(currentImage);
        currentImage = newImage;

        if (currentImage != nullptr)
        {
            enablementChanged();
            addAndMakeVisible(currentImage);
            updateDrawable();
        }
    }
}

void StatusBarButton::updateDrawable()
{
    if (currentImage != nullptr)
    {
        currentImage->setInterceptsMouseClicks(false, false);
        currentImage->setTransformToFit(
            getContentArea().toFloat(),
            juce::RectanglePlacement::xLeft | juce::RectanglePlacement::yMid
        );
        currentImage->setAlpha(isEnabled() ? 1.0f : 0.5f);
    }
}

void StatusBarButton::resized()
{
    ToolbarItemComponent::resized();
    updateDrawable();
}

void StatusBarButton::enablementChanged()
{
    ToolbarItemComponent::enablementChanged();
    updateDrawable();
}

juce::Drawable* StatusBarButton::getImageToUse() const
{
    if (getStyle() == juce::Toolbar::textOnly)
        return nullptr;

    if (getToggleState() && toggledOnImage != nullptr)
        return toggledOnImage.get();

    return normalImage.get();
}

void StatusBarButton::buttonStateChanged()
{
    setCurrentImage(getImageToUse());
}

void StatusBarButton::setButtonNormalIcon(std::unique_ptr<juce::Drawable> normalIm)
{
    this->normalImage = nullptr;
    this->normalImage = std::move(normalIm);
    setCurrentImage(getImageToUse());
}

void StatusBarButton::setButtonToggledIcon(std::unique_ptr<juce::Drawable> toggledIm)
{
    this->toggledOnImage = nullptr;
    this->toggledOnImage = std::move(toggledIm);
    setCurrentImage(getImageToUse());
}
