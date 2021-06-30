#include "Effect.h"
#include "../../SpoolProcessor.h"

Effect::Effect(SpoolProcessor* processor, int index, int track, int sample) : processor(processor), index(index), track(track), sample(sample) {
    juce::ValueTree& tree = Parameters::getValueTree();
    getParamValues(tree);
    tree.addListener(this);
};


void Effect::getParamValues(juce::ValueTree& tree, const juce::Identifier& param) {
    
    if (sample == -1) {
        if (!Parameters::isTrackEffect(tree, track, index)) return;
        wetReal = Parameters::getTrackEffectParam(track, index, Config::Parameters::Wet);
        paramOneReal = Parameters::getTrackEffectParam(track, index, Config::Parameters::ParamOne);
        paramTwoReal = Parameters::getTrackEffectParam(track, index, Config::Parameters::ParamTwo);
    } else {
        if (!Parameters::isTrackSampleEffect(tree, track, sample, index)) return;
        wetReal = Parameters::getTrackSampleEffectParam(track, sample, index, Config::Parameters::Wet);
        paramOneReal = Parameters::getTrackSampleEffectParam(track, sample, index, Config::Parameters::ParamOne);
        paramTwoReal = Parameters::getTrackSampleEffectParam(track, sample, index, Config::Parameters::ParamTwo);
    }
    
    wetMidi = clampValue(wetReal);
    wet = wetMidi / Config::MaxParamValue;
    
    paramOneMidi = clampValue(paramOneReal);
    paramOne = paramOneMidi / Config::MaxParamValue;

    paramTwoMidi = clampValue(paramTwoReal);
    paramTwo = paramTwoMidi / Config::MaxParamValue;
    
    if (param.isNull()) return;
    juce::String paramName = param.toString();
    
    if (paramName == Config::ParameterNames[Config::Parameters::Wet]) return onWetChanged();
    if (paramName == Config::ParameterNames[Config::Parameters::ParamOne]) return onParamOneChanged();
    if (paramName == Config::ParameterNames[Config::Parameters::ParamTwo]) return onParamTwoChanged();

}

void Effect::valueTreePropertyChanged(juce::ValueTree& tree, const juce::Identifier& param) {
    juce::String paramName = param.toString();
    getParamValues(tree, param);
};
