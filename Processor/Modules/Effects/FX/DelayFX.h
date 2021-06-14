#pragma once

#include "../FXBase.h"

class DelayFX : public FXBase {
public:
    DelayFX(int index, int track, int sample = -1) : FXBase{index, track, sample} {
    }
    
    ~DelayFX() {
    }
    
    
    
    void processBlockBefore(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override {
        
    };

    void processBlockAfter(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override {
        if (track == 0) {
//            DBG(wet);
            auto a = 1;
        }
    };
};
