#include "PanningEffect.h"
#include "Processor/SpoolProcessor.h"


PanningEffect::PanningEffect(SpoolProcessor* processor, ParameterValue& wet, ParameterValue& paramA, ParameterValue& paramB, int index, int track, int sample) :
    Effect{ processor, wet, paramA, paramB, index, track, sample }
{}
PanningEffect::~PanningEffect() {
}


void PanningEffect::prepareToPlay(double sampleRate, int samplesPerBlock) {
    
};

void PanningEffect::processBlockBefore(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
    
};

void PanningEffect::processBlockAfter(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
    
};

void PanningEffect::onWetChanged() {
    
};

void PanningEffect::onParamAChanged() {
    
};

void PanningEffect::onParamBChanged() {
    
};
