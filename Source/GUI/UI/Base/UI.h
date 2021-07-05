#pragma once

#include <JuceHeader.h>
#include "GUI/UI/Base/UIComponents.h"

#include "GUI/UI/TopAreaUI.h"
#include "GUI/UI/TracksUI.h"
#include "GUI/UI/SequencerUI.h"
#include "GUI/UI/RMSMeterUI.h"
#include "GUI/UI/ControlButtonsUI.h"
#include "GUI/UI/ControlGroupUI.h"

#include "Processor/Modules/ControlGroup/ControlGroup.h"

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
    ControlGroupUI controlGroupAUI {ControlGroup::Group::A};
    ControlGroupUI controlGroupBUI  {ControlGroup::Group::B};
};
