#pragma once
#include <JuceHeader.h>

class StatusBarButton :
    public juce::ToolbarItemComponent
{
public:
    enum class ShowType {
        Hide = 0,
        Icon = 1 << 0,
        Text = 1 << 1,
        IconAndText = 1 << 0 | 1 << 1
    };

    using ItemStateCheckFunction = std::function<bool(const ShowType&, const ShowType&)>;
    static ItemStateCheckFunction stateCheckFunc;

    StatusBarButton(const int iid, ShowType showType, juce::StringRef buttonText,
        std::unique_ptr<juce::Drawable> normalIm,
        std::unique_ptr<juce::Drawable> toggledOnIm);
    ~StatusBarButton();

    bool getToolbarItemSizes(int toolbarDepth, bool isToolbarVertical, int& preferredSize,
        int& minSize, int& maxSize) override;
    void paintButtonArea(juce::Graphics&, int width, int height, bool isMouseOver, bool isMouseDown) override;
    void contentAreaChanged(const juce::Rectangle<int>&) override;
    void buttonStateChanged() override;
    void resized() override;
    void enablementChanged() override;

    void setButtonNormalIcon(std::unique_ptr<juce::Drawable> normalIm);
    void setButtonToggledIcon(std::unique_ptr<juce::Drawable> toggledIm);

private:
    const ShowType showType = ShowType::IconAndText;
    std::unique_ptr<juce::Drawable> normalImage, toggledOnImage;
    juce::Drawable* currentImage = nullptr;

    void updateDrawable();
    juce::Drawable* getImageToUse() const;
    void setCurrentImage(juce::Drawable*);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StatusBarButton)
};

