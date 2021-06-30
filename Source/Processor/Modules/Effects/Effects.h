#pragma once
#include <JuceHeader.h>
#include "Effect.h"

class SpoolProcessor;

class Effects {
public:
    SpoolProcessor* processor;
    int track = -1;
    int sample = -1;

    juce::OwnedArray<Effect> effects;

    Effects(SpoolProcessor* processor, int track, int sample = -1);
    ~Effects();
    
    void prepareToPlay(double sampleRate, int samplesPerBlock);
    void processBlockBefore(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);
    void processBlockAfter(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);
    
    Effect* getEffect(int index) {
        return effects[index];
    }
};
