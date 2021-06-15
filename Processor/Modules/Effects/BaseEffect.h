
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
    
    juce::String wetParam;
    float wet = 0.0f;
    
    juce::String valueOneParam;
    float valueOne = 0.0f;
    
    juce::String valueTwoParam;
    float valueTwo = 0.0f;
    
    BaseEffect(SpoolProcessor* processor, int index, int track, int sample = -1);
    
    
    void valueTreePropertyChanged(juce::ValueTree& tree, const juce::Identifier& param) override;
    
    virtual ~BaseEffect() {};

    virtual void prepareToPlay(double sampleRate, int samplesPerBlock) {};
    virtual void processBlockBefore(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {};
    virtual void processBlockAfter(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {};
    
    virtual void onWetChanged() {};
    virtual void onValueOneChanged() {};
    virtual void onValueTwoChanged() {};
    
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
