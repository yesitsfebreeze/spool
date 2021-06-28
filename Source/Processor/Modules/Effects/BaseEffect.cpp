#include "BaseEffect.h"
#include "../../SpoolProcessor.h"

BaseEffect::BaseEffect(SpoolProcessor* processor, int index, int track, int sample) : processor(processor), index(index), track(track), sample(sample) {
    wetParam = Parameters::buildParamName(Config::Parameters::EffectParam::Wet, index, track, sample);
    valueOneParam = Parameters::buildParamName(Config::Parameters::EffectParam::ValueOne, index, track, sample);
    valueTwoParam = Parameters::buildParamName(Config::Parameters::EffectParam::ValueTwo, index, track, sample);

    wet = Parameters::get(wetParam);
    valueOne = Parameters::get(valueOneParam);
    valueTwo = Parameters::get(valueTwoParam);

    Parameters::getValueTree().addListener(this);
};


void BaseEffect::valueTreePropertyChanged(juce::ValueTree& tree, const juce::Identifier& param) {
    juce::String paramName = param.toString();
    if (paramName == wetParam) {
        wet = Parameters::get(wetParam);
        onWetChanged();
        return;
    }
    if (paramName == valueOneParam) {
        valueOne = Parameters::get(valueOneParam);
        onValueOneChanged();
        return;
    }
    if (paramName == valueTwoParam) {
        valueTwo = Parameters::get(valueTwoParam);
        onValueTwoChanged();
        return;
    }
};
