#pragma once
#include <JuceHeader.h>

#include "Processor/Modules/Effects/Base/Effect.h"

class SpoolProcessor;

class Effects {
public:
    SpoolProcessor* processor;
    int track = -1;
    int sample = -1;

    juce::OwnedArray<Effect> effects;

    Effects(SpoolProcessor* processor, int track, int sample = -1);
    ~Effects();

    
    template<class EffectClass>
    void addEffect() {
        effects.add(new EffectClass(processor, effects.size(), track, sample));
    };

    void prepareToPlay(double sampleRate, int samplesPerBlock);
    void processBlockBefore(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);
    void processBlockAfter(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);
    
    Effect* getEffect(int index) {
        return effects[index];
    }
};
