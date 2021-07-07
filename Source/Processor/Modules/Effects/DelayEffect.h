#pragma once

#include "Base/Effect.h"

class SpoolProcessor;

class DelayEffect : public Effect {
public:
    DelayEffect(SpoolProcessor* processor, ParameterValue& wet, ParameterValue& paramA, ParameterValue& paramB, int index, int track, int sample = -1);
    ~DelayEffect();
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlockAfter(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override;

    void onParamAChanged() override;

private:
    double sampleRate = -1;
    
    const float minDelayTime = 1000;
    const float maxDelayTime = 24000;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> delayTimeInterpolation;
    
    juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Linear> delayLine;
    std::vector<float> lastOutput{ 0.f, 0.f }; // for storing feedback val
};
