#pragma once

#include <JuceHeader.h>
#include "Base/UIComponents.h"
#include "../../Config.h"

class Track;

class TrackButtonUI : public UIButtonComponent {
public:

    TrackButtonUI(int index) : UIButtonComponent(index) {}

    Track* track;

    bool isColorForced = false;
    bool hasGroupColor = false;
    
    juce::Colour groupBorderColor;
    juce::Colour groupFillColor;
    
    bool isMuted = false;
    bool isPlaying = false;
    bool isSelected = false;
    bool isRecording = false;

    void afterPaint() override;
    void getButtonColors() override;
    void setGroupColor(juce::Colour borderColor, juce::Colour fillColor);
    void removeGroupColor();
    void forceColor(juce::Colour borderColor, juce::Colour fillColor);
    void removeForcedColor();
};



class TracksUI : public UIComponent {
    using Cmd = Config::Command::ID;
    
public:
    TracksUI();
    void resized() override;
    void onSetReferences() override;
    void deselectAllTracks();
    juce::OwnedArray<TrackButtonUI> buttons;
};
