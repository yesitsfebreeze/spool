#pragma once

#include <JuceHeader.h>
#include "Base/UIComponents.h"

class ControlButtonsUI : public UIComponentAnimated {
    
public:
    ControlButtonsUI();
    ~ControlButtonsUI();
    void resized() override;
    void onSetReferences() override;
    
    UIButtonComponent* functionButton;
    UIButtonComponent* muteButton;
    UIButtonComponent* playButton;
    UIButtonComponent* recordButton;
    
    void update() override;
    
private:
    juce::OwnedArray<UIButtonComponent> buttons;
};
