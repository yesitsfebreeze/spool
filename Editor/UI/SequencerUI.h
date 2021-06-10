#pragma once

#include <JuceHeader.h>
#include "../../Config.h"
#include "Base/UIComponents.h"

class SequencerUI : public AnimatedUIComponent {
    
public:
    SequencerUI();
    ~SequencerUI();
    void update() override;
    void paint (juce::Graphics& g) override;
    void resized() override;
    
private:
    bool selected = false;
    bool isRunning = false;
    bool lightState = false;
    bool lightStateChanged = false;
    int sequencerTick = 0;
};
