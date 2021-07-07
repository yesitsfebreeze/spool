
#pragma once

#include <JuceHeader.h>
#include "Processor/Modules/Parameters/Parameteres.h"
#include "Processor/Modules/Parameters/ParameterValue.h"
#include "Config.h"

class SpoolProcessor;

class Effect {
protected:
    SpoolProcessor* processor;
    ParameterValue& wet;
    ParameterValue& paramA;
    ParameterValue& paramB;
    
private:
    int index = -1;
    
public:
    
    int track = -1;
    int sample = -1;

    bool selected = false;
    
    Effect(SpoolProcessor* processor, ParameterValue& wet, ParameterValue& paramA, ParameterValue& paramB, int index, int track, int sample = -1);
    
    virtual ~Effect() {};

    virtual void prepareToPlay(double sampleRate, int samplesPerBlock) {};
    virtual void processBlockBefore(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {};
    virtual void processBlockAfter(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {};
    
    virtual void setupDefaultParams() {
        wet.set(0);
        paramA.set(0);
        paramB.set(0);
    };
    
    virtual void setupParamValueListeneres() {
        wet.onChange = [this] (ParameterValue& value) { onWetChanged(); };
        paramA.onChange = [this] (ParameterValue& value) { onParamAChanged(); };
        paramB.onChange = [this] (ParameterValue& value) { onParamBChanged(); };
    };

    virtual void onWetChanged() {};
    virtual void onParamAChanged() {};
    virtual void onParamBChanged() {};
    
    int getIndex() {
        return index;
    }
    
    Config::EffectID getEnum() {
        return (Config::EffectID) index;
    }
    
    bool isSampleEffect() {
        return sample != -1;
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
