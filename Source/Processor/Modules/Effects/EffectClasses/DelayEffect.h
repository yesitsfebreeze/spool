#pragma once

#include "../Effect.h"

class SpoolProcessor;

class DelayEffect : public Effect {
public:
    DelayEffect(SpoolProcessor* processor, int index, int track, int sample = -1);
    ~DelayEffect();
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlockBefore(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override;
    void processBlockAfter(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override;

    void onWetChanged();
    void onParamOneChanged();
    void onParamTwoChanged();

private:
    double mSampleRate = 48000;
    juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Linear> delayLine;
    
    juce::Atomic<float> wetMix      { 0.f };
    juce::Atomic<float> delayTime   { 24000.f };
    juce::Atomic<float> feedback    { 0.3f };
    juce::SmoothedValue<float, ValueSmoothingTypes::Linear> delaySmoothed;
    std::vector<float> lastOutput{ 0.f, 0.f }; // for storing feedback val
};
