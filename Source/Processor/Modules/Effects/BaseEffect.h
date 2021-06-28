
#pragma once

#include <JuceHeader.h>
#include "../../Parameteres.h"
#include "../../../Config.h"

class SpoolProcessor;

class BaseEffect : public juce::ValueTree::Listener {
public:
    SpoolProcessor* processor;

    int index = -1;
    int track = -1;
    int sample = -1;
    

    bool selected = false;

    float wetReal = 0.0f;
    float wetMidi = 0.0f;
    float wet = 0.0f;
    
    float paramOneReal = 0.0f;
    float paramOneMidi = 0.0f;
    float paramOne = 0.0f;
    
    float paramTwoReal = 0.0f;
    float paramTwoMidi = 0.0f;
    float paramTwo = 0.0f;
    
    BaseEffect(SpoolProcessor* processor, int index, int track, int sample = -1);
    
    void getParamValues(juce::ValueTree& tree, const juce::Identifier& param = juce::Identifier::null);
    void valueTreePropertyChanged(juce::ValueTree& tree, const juce::Identifier& param) override;
    
    virtual ~BaseEffect() {};

    virtual void prepareToPlay(double sampleRate, int samplesPerBlock) {};
    virtual void processBlockBefore(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {};
    virtual void processBlockAfter(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {};
    
    virtual void onWetChanged() {};
    virtual void onParamOneChanged() {};
    virtual void onParamTwoChanged() {};
    
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
