#pragma once
#include <JuceHeader.h>
#include "FXBase.h"


class Effects {
public:
    int track = -1;
    int sample = -1;
    juce::OwnedArray<FXBase> effects;
    
    Effects(int track, int sample = -1);
    ~Effects();
    
    void processBlockBefore(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);
    void processBlockAfter(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);

    // do for all
    // do for selected
};
