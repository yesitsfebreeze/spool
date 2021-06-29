#pragma once

#include "Base/UIComponents.h"
#include "../../Config.h"
#include "../EditorConfig.h"

class ControlGroupUI : public UIComponent {
  
public:
    int index;

    ControlGroupUI(int index) {
        this->index = index;
        
        if (index == 0) {
            groupColor = EditorConfig::Colors::GroupColorOne;
        }
        
        if (index == 1) {
            groupColor = EditorConfig::Colors::GroupColorTwo;
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
