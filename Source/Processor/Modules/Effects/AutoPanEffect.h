#pragma once

#include <JuceHeader.h>
#include "Base/Effect.h"

class AutoPanEffect : public Effect {
    
public:
    
    AutoPanEffect(SpoolProcessor* processor, ParameterValue& wet, ParameterValue& paramA, ParameterValue& paramB, int index, int track, int sample = -1);
    ~AutoPanEffect();
    
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlockAfter(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override;
    void onParamAChanged() override;

private:
    const float minFrequency = 0.25f;
    const float maxFrequency = 10.0f;
 
    static constexpr size_t lfoUpdateRate = 128;
    juce::dsp::Oscillator<float> lfo;
    
//    float rndOneSpeed = 1.2f;
//    float rndTwoSpeed = 1.05f;
//    float rndThreeSpeed = 0.92f;
//    float rndFourSpeed = 0.84f;
//    
//    juce::dsp::Oscillator<float> rndLfoOne;
//    juce::dsp::Oscillator<float> rndLfoTwo;
//    juce::dsp::Oscillator<float> rndLfoThree;
//    juce::dsp::Oscillator<float> rndLfoFour;

};

