#pragma once

#include <JuceHeader.h>

class FXBase {
public:
    float wet = 0.0f;
    float paramOne = 0.0f;
    float paramTwo = 0.0f;
    
    FXBase() {
        
    };
    ~FXBase() {
        
    };
    
    
 
    virtual void processBlockBefore(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
        
    };
    virtual void processBlockAfter(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
        
    };
};
