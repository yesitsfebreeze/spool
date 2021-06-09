#pragma once
#include <JuceHeader.h>
#include "FXBase.h"


class Effects {
public:
    juce::OwnedArray<FXBase> effects;
    
    Effects();
    ~Effects();
    
    void processBlockBefore(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);
    
    void processBlockAfter(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);
};
