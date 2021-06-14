#pragma once

#include <JuceHeader.h>
#include "../../Config.h"
#include "Base/UIComponents.h"

class RMSMeterUI : public AnimatedUIComponent {
    
public:
    RMSMeterUI();
    ~RMSMeterUI();
    void update() override;
    void paint (juce::Graphics& g) override;
    void resized() override;
};
