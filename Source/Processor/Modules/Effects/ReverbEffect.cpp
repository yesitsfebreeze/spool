    #include "ReverbEffect.h"

ReverbEffect::ReverbEffect(SpoolProcessor* processor, ParameterValue& wet, ParameterValue& paramA, ParameterValue& paramB, int index, int track, int sample) :
    Effect{ processor, wet, paramA, paramB, index, track, sample }
{}

ReverbEffect::~ReverbEffect() {}

void ReverbEffect::prepareToPlay(double sampleRate, int samplesPerBlock) {
    reverb.setParameters(reverbParameters);
    reverb.setSampleRate(sampleRate);
}

void ReverbEffect::processBlockAfter(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
    auto CHLeftWritePointer = buffer.getWritePointer(Config::CHLeft);
    auto CHRightWritePointer = buffer.getWritePointer(Config::CHRight);
    int numSamples = buffer.getNumSamples();

    reverb.processStereo(CHLeftWritePointer, CHRightWritePointer, numSamples);
}

void ReverbEffect::onWetChanged() {
    reverbParameters.wetLevel = wet.percent;
    reverbParameters.dryLevel = 1 - wet.percent;
    reverb.setParameters(reverbParameters);
}

void ReverbEffect::onParamAChanged() {
    reverbParameters.roomSize = paramA.percent;
    reverb.setParameters(reverbParameters);
}

void ReverbEffect::onParamBChanged() {
    reverbParameters.width = paramB.percent;
    reverb.setParameters(reverbParameters);
}

