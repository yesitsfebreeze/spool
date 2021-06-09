#pragma once

#include "../FXBase.h"

class DelayFX : public FXBase {
public:
    DelayFX() {
        
    }
    ~DelayFX() {
        
    }
    
    void processBlockBefore(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override {
        
    };

    void processBlockAfter(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override {
//        DBG(wet);
    };
};
