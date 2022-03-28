#pragma once
#include <JuceHeader.h>
#include "TrackView.h"
#include "NoteView.h"
#include "ParamView.h"

class EditorWidget final :
    public juce::Component
{
public:
    EditorWidget();
    ~EditorWidget();

    void init();

    enum class ActivedWidget
    {
        TrackView = 0,
        NoteView = 1,
        ParamView = 2
    };
    void active(bool actived, ActivedWidget widget);

protected:
    void resized()override;

private:
    std::unique_ptr<TrackView> trackView;
    std::unique_ptr<NoteView> noteView;
    std::unique_ptr<ParamView> paramView;

    std::unique_ptr<juce::StretchableLayoutManager> stretchableManager;
    std::unique_ptr<juce::StretchableLayoutResizerBar> stretchableBar0, stretchableBar1;

    std::vector<juce::Component*> components;

    void reStre();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EditorWidget)
};

