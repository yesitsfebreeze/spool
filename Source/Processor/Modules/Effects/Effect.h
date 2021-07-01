
#pragma once

#include <JuceHeader.h>
#include "../../Modules/Parameters/Parameteres.h"
#include "../../../Config.h"

class SpoolProcessor;

class Effect : public juce::ValueTree::Listener {
public:
    SpoolProcessor* processor;

    int index = -1;
    int track = -1;
    int sample = -1;
    

    bool selected = false;

    float wetReal = 0.0f;
    float wetMidi = 0.0f;
    float wet = 0.0f;
    
    float paramAReal = 0.0f;
    float paramAMidi = 0.0f;
    float paramA = 0.0f;
    
    float paramBReal = 0.0f;
    float paramBMidi = 0.0f;
    float paramB = 0.0f;
    
    Effect(SpoolProcessor* processor, int index, int track, int sample = -1);
    
    void getParamValues(juce::ValueTree& tree, const juce::Identifier& param = juce::Identifier::null);
    void valueTreePropertyChanged(juce::ValueTree& tree, const juce::Identifier& param) override;
    
    virtual ~Effect() {};

    virtual void prepareToPlay(double sampleRate, int samplesPerBlock) {};
    virtual void processBlockBefore(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {};
    virtual void processBlockAfter(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {};
    
    virtual void onWetChanged() {};
    virtual void onParamAChanged() {};
    virtual void onParamBChanged() {};
    
    juce::var clampValue(juce::var value) {
        if (value.isDouble()) {
            if ((double) value >= Config::MaxParamValue) return Config::MaxParamValue;
            if ((double) value <= Config::MinParamValue) return Config::MinParamValue;
        }
        return value;
    }
    
    bool toggleSelect() {
        selected = !selected;
        return selected;
    }

    bool select() {
        selected = true;
        return selected;
    }

    bool deselect() {
        selected = false;
        return selected;
    }

    bool IsSelect() {
        return selected;
    }
};
