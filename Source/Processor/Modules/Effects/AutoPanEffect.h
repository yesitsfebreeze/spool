#pragma once

#include <JuceHeader.h>
#include "Processor/Modules/Effects/Base/Effect.h"
#include "Processor/Utils/RandomLFO.h"

class AutoPanEffect : public Effect {
    
public:
    
    AutoPanEffect(SpoolProcessor* processor, ParameterValue& wet, ParameterValue& paramA, ParameterValue& paramB, int index, int track, int sample = -1);
    ~AutoPanEffect();
    
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlockAfter(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override;
    void onParamAChanged() override;
    void onParamBChanged() override;

private:
    const float minFrequency = 0.25f;
    const float maxFrequency = 10.0f;
 
    static constexpr size_t LFOUpdateRate = 128;
    float targetFrequency = minFrequency;
    juce::dsp::Oscillator<float> LFO;
    
    RandomLFO randomLFO;
};

