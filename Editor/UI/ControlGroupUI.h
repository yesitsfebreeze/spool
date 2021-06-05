#pragma once

#include "Base/UIComponents.h"
#include "../../Config.h"

class ControlGroupUI : public UIComponent {
  
public:
    int index;
    juce::OwnedArray<UIKnobComponent> knobs;

    ControlGroupUI(int index) {
        this->index = index;
        initializeKnobs();
    }
    
    void initializeKnobs();

    void paint(juce::Graphics& g) override {}
    
    void resized() override;
};
