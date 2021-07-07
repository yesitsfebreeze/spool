#pragma once

#include <JuceHeader.h>

#include "Config.h"
#include "Processor/Modules/Parameters/ParameterValue.h"

class Parameters {
    
public:
    Parameters& operator=(const Parameters&) = delete;
    Parameters(const Parameters&) = delete;
    
    static Parameters& instance() {
        static Parameters _instance;
        return _instance;
    }

    static std::map<juce::String, ParameterValue*>& getParams() {
        return instance()._getParams();
    }
    
    static ParameterValue& get(juce::String identifier) {
        try {
            return *getParams().at(identifier);
        } catch (std::out_of_range e) {
            getParams()[identifier] = new ParameterValue();
            return *getParams().at(identifier);
        }
    }
    
    static ParameterValue& get(Config::Parameter param) {
        return get(getIdentifier(param));
    }
    
    static ParameterValue& get(Config::TrackID trackId, Config::Parameter param) {
        return get(getIdentifier(trackId, param));
    }
    
    static ParameterValue& get(Config::TrackID trackId, Config::EffectID effectId, Config::Parameter param) {
        return get(getIdentifier(trackId, effectId, param));
    }
    
    static ParameterValue& get(Config::TrackID trackId, Config::SampleID sampleId, Config::Parameter param) {
        return get(getIdentifier(trackId, sampleId, param));
    }
    
    static ParameterValue& get(Config::TrackID trackId, Config::SampleID sampleId, Config::EffectID effectId, Config::Parameter param) {
        return get(getIdentifier(trackId, sampleId, effectId, param));
    }
        
    ~Parameters() {
        params.reset();
    };
    
private:
    Parameters() {
        params.reset(new std::map<juce::String, ParameterValue*>);
    };
    
    std::map<juce::String, ParameterValue*>& _getParams() {
        return *params;
    }
    
    static Parameters _instance;

    std::unique_ptr<std::map<juce::String,ParameterValue*>> params;
    
    
    static juce::String getIdentifier(Config::Parameter param) {
        return Config::ParameterName[param];
    }
    
    static juce::String getIdentifier(Config::TrackID trackId, Config::Parameter param) {
        return "Track" + juce::String(trackId) + Config::ParameterName[param];
    }

    static juce::String getIdentifier(Config::TrackID trackId, Config::EffectID effectId, Config::Parameter param) {
        return "track" + juce::String(trackId) + "effect" + juce::String(effectId) + Config::ParameterName[param];
    }
    
    static juce::String getIdentifier(Config::TrackID trackId, Config::SampleID sampleId, Config::Parameter param) {
        return "track" + juce::String(trackId) + "sample" + juce::String(sampleId) + Config::ParameterName[param];
    }

    static juce::String getIdentifier(Config::TrackID trackId, Config::SampleID sampleId, Config::EffectID effectId, Config::Parameter param) {
        return "track" + juce::String(trackId) + "sample" + juce::String(sampleId) + "effect" + juce::String(effectId) + Config::ParameterName[param];
    }
    
};


