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
    void onParamBChanged() override;
private:
    double sampleRate = -1;
    
    juce::Reverb reverb;
    juce::Reverb::Parameters  reverbParameters {
        0.0f,    // Room size, 0 to 1.0, where 1.0 is big, 0 is small.
        0.0f,    // Damping, 0 to 1.0, where 0 is not damped, 1.0 is fully damped.
        0.0f,   // Wet level, 0 to 1.0
        1.0f,    // Dry level, 0 to 1.0
        0.0f,    // Reverb width, 0 to 1.0, where 1.0 is very wide.
        0.0f,    // Freeze mode - values < 0.5 are "normal" mode, values > 0.5
    };

};
