#pragma once

#include <JuceHeader.h>
#include "Config.h"

#include "GUI/UI/Base/UIComponents.h"

class RMSMeterUI : public UIComponentAnimated {
    
public:
    RMSMeterUI();
    ~RMSMeterUI();
    void update() override;
    void paint (juce::Graphics& g) override;
    void resized() override;
};
