#pragma once

#include "../BaseEffect.h"

class SpoolProcessor;

class DelayEffect : public BaseEffect {
public:
    DelayEffect(SpoolProcessor* processor, int index, int track, int sample = -1);
    ~DelayEffect();
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlockAfter(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override;
};
