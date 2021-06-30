#pragma once

#include "Base/UIComponents.h"
#include "../../Config.h"
#include "../GUIConfig.h"

class ControlGroupUI : public UIComponent {
  
public:
    int index;

    ControlGroupUI(int index) {
        this->index = index;
        
        if (index == 0) {
            groupColor = GUIConfig::Colors::GroupColorOne;
        }
        
        if (index == 1) {
            groupColor = GUIConfig::Colors::GroupColorTwo;
        }
        initializeKnobs();
    }
    
    void initializeKnobs();
    
    void addToGroup();
    
    void addVolumeKnob();
    void addParamTwoKnob();
    void addParamOneKnob();
    void addDryWetKnob();

    void paint(juce::Graphics& g) override {}
    
    void resized() override;
    
private:
    juce::OwnedArray<UIKnobComponent> knobs;
    juce::Colour groupColor;
};
