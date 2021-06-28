#pragma once

#include <JuceHeader.h>
#include "../Config.h"

class Parameters {
public:
    Parameters& operator=(const Parameters&) = delete;
    Parameters(const Parameters&) = delete;
    
    static Parameters& instance() {
        static Parameters _instance;
        return _instance;
    }
    
    static juce::ValueTree& getValueTree() {
        return instance()._getValueTree();
    }

    static void set(juce::String name, juce::var value) {
        instance()._set(name, value);
    }

    static juce::var get(juce::String name) {
        return instance()._get(name);
    }
    
    static juce::String buildParamName(int paramId, int effect = -1, int track = -1, int sample = -1) {
        juce::String name = "";
        
        if (track != -1) name = name + "t" + std::to_string(track);
        if (sample != -1) name = name + "s" + std::to_string(sample);
        if (effect != -1) name = name + "e" + std::to_string(effect);
        
        name = name + "p" + std::to_string(paramId);
        
        return name;
    }

    
    ~Parameters() {
        params.reset();
    };
    
private:
    Parameters() {
        params.reset(new juce::ValueTree("Parameters"));
    };
    
    juce::ValueTree& _getValueTree() {
        return *params;
    }
    
    void _set(juce::String name, juce::var value) {
        params->setProperty(name, value, nullptr);
    }
    
    juce::var _get(juce::String name) {
        return params->getProperty(name);
    }
    
    static Parameters _instance;
    
    std::unique_ptr<juce::ValueTree> params;
};


