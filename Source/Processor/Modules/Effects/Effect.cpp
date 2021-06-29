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
        wetReal.set(Parameters::getTrackEffectParam(track, index, Config::Parameters::Wet));
        paramOneReal.set(Parameters::getTrackEffectParam(track, index, Config::Parameters::ParamOne));
        paramTwoReal.set(Parameters::getTrackEffectParam(track, index, Config::Parameters::ParamTwo));
    } else {
        if (!Parameters::isTrackSampleEffect(tree, track, sample, index)) return;
        wetReal.set(Parameters::getTrackSampleEffectParam(track, sample, index, Config::Parameters::Wet));
        paramOneReal.set(Parameters::getTrackSampleEffectParam(track, sample, index, Config::Parameters::ParamOne));
        paramTwoReal.set(Parameters::getTrackSampleEffectParam(track, sample, index, Config::Parameters::ParamTwo));
    }
    
    wetMidi.set(clampValue(wetReal.get()));
    wet.set(wetMidi.get() / Config::MaxParamValue);
    
    paramOneMidi.set(clampValue(paramOneReal.get()));
    paramOne.set(paramOneMidi.get() / Config::MaxParamValue);

    paramTwoMidi.set(clampValue(paramTwoReal.get()));
    paramTwo.set(paramTwoMidi.get() / Config::MaxParamValue);
    
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
