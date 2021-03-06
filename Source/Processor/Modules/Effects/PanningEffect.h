#pragma once

#include "Base/Effect.h"

class SpoolProcessor;

class PanningEffect : public Effect {
public:
    PanningEffect(SpoolProcessor* processor, ParameterValue& wet, ParameterValue& paramA, ParameterValue& paramB, int index, int track, int sample = -1);
    ~PanningEffect();
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlockBefore(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override;
    void processBlockAfter(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override;

    void onWetChanged() override;
    void onParamAChanged() override;
    void onParamBChanged() override;
};
