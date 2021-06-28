#pragma once

#include "../BaseEffect.h"

class SpoolProcessor;

class PanningEffect : public BaseEffect {
public:
    PanningEffect(SpoolProcessor* processor, int index, int track, int sample = -1);
    ~PanningEffect();
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlockBefore(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override;
    void processBlockAfter(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override;

    void onWetChanged() override;
    void onValueOneChanged() override;
    void onValueTwoChanged() override;
    
private:
    juce::dsp::Panner<float> panner;
};
