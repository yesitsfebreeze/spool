#pragma once

#include <JuceHeader.h>
#include "Base/UIComponents.h"

class TopAreaUI : public UIComponent {
    
public:
    std::unique_ptr<UIKnobComponent> bypassKnob;

    TopAreaUI() {
        initializeKnobs();
    }
    ~TopAreaUI() override;
    
    void initializeKnobs();

    void paint(juce::Graphics& g) override {}
    
    void resized() override;
};
