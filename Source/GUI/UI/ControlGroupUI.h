#pragma once

#include "Base/UIComponents.h"
#include "../../Config.h"
#include "../GUIConfig.h"
#include "../../Processor/Modules/ControlGroup/ControlGroup.h"

class ControlGroupUI : public UIComponent {
  
public:
    ControlGroup::Group groupID;


    ControlGroupUI(ControlGroup::Group groupID) : groupID(groupID) {
        if (groupID == ControlGroup::Group::A) groupColor = GUIConfig::Colors::GroupColorA;
        if (groupID == ControlGroup::Group::B) groupColor = GUIConfig::Colors::GroupColorB;

        initializeKnobs();
    }
    
    void initializeKnobs();
    
    void addToGroup();
    
    void addVolumeKnob();
    void addParamAKnob();
    void addParamBKnob();
    void addDryWetKnob();

    void paint(juce::Graphics& g) override {}
    
    void resized() override;
    
private:
    juce::OwnedArray<UIKnobComponent> knobs;
    juce::Colour groupColor;
};
