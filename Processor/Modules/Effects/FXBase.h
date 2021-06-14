
#pragma once

#include <JuceHeader.h>
#include "../../Parameteres.h"
#include "../../../Config.h"

class FXBase : public juce::ValueTree::Listener {
public:
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
    
    FXBase(int index, int track, int sample = -1) : index(index), track(track), sample(sample) {
        wetParam = Parameters::buildParamName(Config::Parameters::EffectParam::Wet, index, track, sample);
        valueOneParam = Parameters::buildParamName(Config::Parameters::EffectParam::ValueOne, index, track, sample);
        valueTwoParam = Parameters::buildParamName(Config::Parameters::EffectParam::ValueTwo, index, track, sample);

        wet = Parameters::get(wetParam);
        valueOne = Parameters::get(valueOneParam);
        valueTwo = Parameters::get(valueTwoParam);

        Parameters::getValueTree().addListener(this);
    };
    
    
    void valueTreePropertyChanged(juce::ValueTree& tree, const juce::Identifier& param) override {
        juce::String paramName = param.toString();
        if (paramName == wetParam) {
            wet = Parameters::get(wetParam);
            DBG(wet);
            return;
        }
        if (paramName == valueOneParam) {
            valueOne = Parameters::get(valueOneParam);
            DBG(valueOne);
            return;
        }
        if (paramName == valueTwoParam) {
            valueTwo = Parameters::get(valueTwoParam);
            DBG(valueTwo);
            return;
        }
    };
    
    virtual ~FXBase() {
        
    };

    virtual void processBlockBefore(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
        
    };
    virtual void processBlockAfter(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
        
    };
    
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
