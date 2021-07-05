#pragma once

#include <JuceHeader.h>

#include "Config.h"

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
    
    static void setParam(int paramId, juce::var value) {
        juce::ValueTree& tree = getValueTree();
        tree.setProperty(Config::ParameterNames[paramId], value, nullptr);
    }
    
    static juce::var getParam(int paramId) {
        juce::ValueTree& tree = getValueTree();
        return tree.getProperty(Config::ParameterNames[paramId]);
    }
    
    static void increaseParam(int paramId) {
        juce::var value = Parameters::getParam(paramId);
        Parameters::setParam(paramId, (double) value + Config::ParamChangePerStep);
    }
    
    static void decreaseParam(int trackId, int paramId) {
        juce::var value = Parameters::getTrackParam(trackId, paramId);
        Parameters::setParam(paramId, (double) value - Config::ParamChangePerStep);
    }
    
    static void setTrackParam(int trackId, int paramId, juce::var value) {
        juce::ValueTree& tree = getValueTree();
        juce::ValueTree trackChild = tree.getOrCreateChildWithName("Track" + juce::String(trackId), nullptr);
        trackChild.setProperty(Config::ParameterNames[paramId], value, nullptr);
    }
    
    static juce::var getTrackParam(int trackId, int paramId) {
        juce::ValueTree& tree = getValueTree();
        juce::ValueTree trackChild = tree.getOrCreateChildWithName("Track" + juce::String(trackId), nullptr);
        return trackChild.getProperty(Config::ParameterNames[paramId]);
    }
    
    static void increaseTrackParam(int trackId, int paramId) {
        juce::var value = Parameters::getTrackParam(trackId, paramId);
        Parameters::setTrackParam(trackId, paramId, (double) value + Config::ParamChangePerStep);
    }
    
    static void decreaseTrackParam(int trackId, int paramId) {
        juce::var value = Parameters::getTrackParam(trackId, paramId);
        Parameters::setTrackParam(trackId, paramId, (double) value - Config::ParamChangePerStep);
    }
    
    static void setTrackEffectParam(int trackId, int effectId, int paramId, juce::var value) {
        juce::ValueTree& tree = getValueTree();
        juce::ValueTree trackChild = tree.getOrCreateChildWithName("Track" + juce::String(trackId), nullptr);
        juce::ValueTree effectChild = trackChild.getOrCreateChildWithName("Effect" + juce::String(effectId), nullptr);
        effectChild.setProperty(Config::ParameterNames[paramId], value, nullptr);
    }
    
    static juce::var getTrackEffectParam(int trackId, int effectId, int paramId) {
        juce::ValueTree& tree = getValueTree();
        juce::ValueTree trackChild = tree.getOrCreateChildWithName("Track" + juce::String(trackId), nullptr);
        juce::ValueTree effectChild = trackChild.getOrCreateChildWithName("Effect" + juce::String(effectId), nullptr);
        return effectChild.getProperty(Config::ParameterNames[paramId]);
    }
    
    static void increaseTrackEffectParam(int trackId, int effectId, int paramId) {
        juce::var value = Parameters::getTrackEffectParam(trackId, effectId, paramId);
        Parameters::setTrackEffectParam(trackId, effectId, paramId, (double) value + Config::ParamChangePerStep);
    }
    
    static void decreaseTrackEffectParam(int trackId, int effectId, int paramId) {
        juce::var value = Parameters::getTrackEffectParam(trackId, effectId, paramId);
        Parameters::setTrackEffectParam(trackId, effectId, paramId, (double) value - Config::ParamChangePerStep);
    }
    
    static void setTrackSampleParam(int trackId, int sampleId, int paramId, juce::var value) {
        juce::ValueTree& tree = getValueTree();
        juce::ValueTree trackChild = tree.getOrCreateChildWithName("Track" + juce::String(trackId), nullptr);
        juce::ValueTree sampleChild = trackChild.getOrCreateChildWithName("Sample" + juce::String(sampleId), nullptr);
        sampleChild.setProperty(Config::ParameterNames[paramId], value, nullptr);
    }
    
    static juce::var getTrackSampleParam(int trackId, int sampleId, int paramId) {
        juce::ValueTree& tree = getValueTree();
        juce::ValueTree trackChild = tree.getOrCreateChildWithName("Track" + juce::String(trackId), nullptr);
        juce::ValueTree sampleChild = trackChild.getOrCreateChildWithName("Sample" + juce::String(sampleId), nullptr);
        return sampleChild.getProperty(Config::ParameterNames[paramId]);
    }
    
    static void increaseTrackSampleParam(int trackId, int sampleId, int paramId) {
        juce::var value = Parameters::getTrackSampleParam(trackId, sampleId, paramId);
        Parameters::setTrackSampleParam(trackId, sampleId, paramId, (double) value + Config::ParamChangePerStep);
    }
    
    static void decreaseTrackSampleParam(int trackId, int sampleId, int paramId) {
        juce::var value = Parameters::getTrackSampleParam(trackId, sampleId, paramId);
        Parameters::setTrackSampleParam(trackId, sampleId, paramId, (double) value - Config::ParamChangePerStep);
    }
    
    static void setTrackSampleEffectParam(int trackId, int sampleId, int effectId, int paramId, juce::var value) {
        juce::ValueTree& tree = getValueTree();
        juce::ValueTree trackChild = tree.getOrCreateChildWithName("Track" + juce::String(trackId), nullptr);
        juce::ValueTree sampleChild = trackChild.getOrCreateChildWithName("Sample" + juce::String(sampleId), nullptr);
        juce::ValueTree effectChild = trackChild.getOrCreateChildWithName("Effect" + juce::String(effectId), nullptr);
        effectChild.setProperty(Config::ParameterNames[paramId], value, nullptr);
    }
    
    static juce::var getTrackSampleEffectParam(int trackId, int sampleId, int effectId, int paramId) {
        juce::ValueTree& tree = getValueTree();
        juce::ValueTree trackChild = tree.getOrCreateChildWithName("Track" + juce::String(trackId), nullptr);
        juce::ValueTree sampleChild = trackChild.getOrCreateChildWithName("Sample" + juce::String(sampleId), nullptr);
        juce::ValueTree effectChild = trackChild.getOrCreateChildWithName("Effect" + juce::String(effectId), nullptr);
        return effectChild.getProperty(Config::ParameterNames[paramId]);
    }
    
    
    static void increaseTrackSampleEffectParam(int trackId, int sampleId, int effectId, int paramId) {
        juce::var value = Parameters::getTrackSampleEffectParam(trackId, sampleId, effectId, paramId);
        Parameters::setTrackSampleEffectParam(trackId, sampleId, effectId, paramId, (double) value + Config::ParamChangePerStep);
    }
    
    static void decreaseTrackSampleEffectParam(int trackId, int sampleId, int effectId, int paramId) {
        juce::var value = Parameters::getTrackSampleEffectParam(trackId, sampleId, effectId, paramId);
        Parameters::setTrackSampleEffectParam(trackId, sampleId, effectId, paramId, (double) value - Config::ParamChangePerStep);
    }
    
    static bool isTrack(juce::ValueTree tree, int trackId) {
        juce::String trackType = tree.getType().toString();
        bool isTrack = trackType == "Track" + juce::String(trackId);
        return isTrack;
    }
    
    static bool isTrackEffect(juce::ValueTree tree, int trackId, int effectId) {
        juce::String effectType = tree.getType().toString();
        juce::String trackType = tree.getParent().getType().toString();
        
        bool isEffect = effectType == "Effect" + juce::String(effectId);
        bool isTrack = trackType == "Track" + juce::String(trackId);
        return isTrack && isEffect;
    }
    
    static bool isTrackSample(juce::ValueTree tree, int trackId, int sampleId) {
        juce::String sampleType = tree.getType().toString();
        juce::String trackType = tree.getParent().getType().toString();
        
        bool isSample = sampleType == "Sample" + juce::String(sampleId);
        bool isTrack = trackType == "Track" + juce::String(trackId);
        return isTrack && isSample;
    }
    
    static bool isTrackSampleEffect(juce::ValueTree tree, int trackId, int sampleId, int effectId) {
        juce::String effectType = tree.getType().toString();
        juce::String sampleType = tree.getParent().getType().toString();
        juce::String trackType = tree.getParent().getParent().getType().toString();
        
        bool isEffect = effectType == "Effect" + juce::String(effectId);
        bool isSample = sampleType == "Sample" + juce::String(sampleId);
        bool isTrack = trackType == "Track" + juce::String(trackId);
        return isEffect && isTrack && isSample;
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


