#pragma once

#include <JuceHeader.h>
#include "../../Config.h"
#include "Base/UIComponents.h"

class PeakMeterUI : public AnimatedUIComponent {
    
public:
    PeakMeterUI();
    ~PeakMeterUI();
    void update() override;
    void paint (juce::Graphics& g) override;
    void resized() override;
};
