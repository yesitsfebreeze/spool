#include "Effect.h"

#include "Processor/SpoolProcessor.h"

Effect::Effect(SpoolProcessor* processor, int index, int track, int sample) : processor(processor), index(index), track(track), sample(sample) {
    juce::ValueTree& tree = Parameters::getValueTree();
    getParamValues(tree);
    tree.addListener(this);
};


void Effect::getParamValues(juce::ValueTree& tree, const juce::Identifier& param) {
    
    if (sample == -1) {
        if (!Parameters::isTrackEffect(tree, track, index)) return;
        wetReal = Parameters::getTrackEffectParam(track, index, Config::Parameters::Wet);
        paramAReal = Parameters::getTrackEffectParam(track, index, Config::Parameters::ParamA);
        paramBReal = Parameters::getTrackEffectParam(track, index, Config::Parameters::ParamB);
    } else {
        if (!Parameters::isTrackSampleEffect(tree, track, sample, index)) return;
        wetReal = Parameters::getTrackSampleEffectParam(track, sample, index, Config::Parameters::Wet);
        paramAReal = Parameters::getTrackSampleEffectParam(track, sample, index, Config::Parameters::ParamA);
        paramBReal = Parameters::getTrackSampleEffectParam(track, sample, index, Config::Parameters::ParamB);
    }
    
    wetMidi = clampValue(wetReal);
    wet = wetMidi / Config::MaxParamValue;
    
    paramAMidi = clampValue(paramAReal);
    paramA = paramAMidi / Config::MaxParamValue;

    paramBMidi = clampValue(paramBReal);
    paramB = paramBMidi / Config::MaxParamValue;
    
    if (param.isNull()) return;
    juce::String paramName = param.toString();
    
    if (paramName == Config::ParameterNames[Config::Parameters::Wet]) return onWetChanged();
    if (paramName == Config::ParameterNames[Config::Parameters::ParamA]) return onParamAChanged();
    if (paramName == Config::ParameterNames[Config::Parameters::ParamB]) return onParamBChanged();

}

void Effect::valueTreePropertyChanged(juce::ValueTree& tree, const juce::Identifier& param) {
    juce::String paramName = param.toString();
    getParamValues(tree, param);
};
