#pragma once
#include "Base/Effect.h"

class SpoolProcessor;

class ReverbEffect : public Effect {
public:
    ReverbEffect(SpoolProcessor* processor, ParameterValue& wet, ParameterValue& paramA, ParameterValue& paramB, int index, int track, int sample = -1);
    ~ReverbEffect();
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlockAfter(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override;

    void onWetChanged() override;
    void onParamAChanged() override;
private:
    double sampleRate = -1;
    
    juce::Reverb reverb;
    juce::Reverb::Parameters  reverbParameters;

};