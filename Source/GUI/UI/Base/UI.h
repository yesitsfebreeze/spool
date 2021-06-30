#pragma once

#include <JuceHeader.h>
#include "UIComponents.h"
#include "../TopAreaUI.h"
#include "../TracksUI.h"
#include "../SequencerUI.h"
#include "../RMSMeterUI.h"
#include "../ControlButtonsUI.h"
#include "../ControlGroupUI.h"

class UI : public UIComponent  {
    
public:
    UI();
    ~UI() override;
    void paint (juce::Graphics& g) override;
    void resized() override;
    void onSetReferences() override;
    void setSquare();
    

    TopAreaUI topAreaUI;
    TracksUI tracksUI;
    SequencerUI sequencerUI;
    RMSMeterUI RMSMeterUI;
    ControlButtonsUI controlButtonsUI;
    ControlGroupUI controlGroupOneUI {0};
    ControlGroupUI controlGroupTwoUI  {1};
};
