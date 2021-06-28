#include "PanningEffect.h"
#include "../../../SpoolProcessor.h"


PanningEffect::PanningEffect(SpoolProcessor* processor, int index, int track, int sample) : BaseEffect{ processor, index, track, sample } {
}

PanningEffect::~PanningEffect() {
}


void PanningEffect::prepareToPlay(double sampleRate, int samplesPerBlock) {
    panner.prepare(processor->processSpec);
    panner.setPan(0);
};

void PanningEffect::processBlockBefore(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
    
};


void PanningEffect::onWetChanged() {
    
};

void PanningEffect::onValueOneChanged() {
    
};

void PanningEffect::onValueTwoChanged() {
    
};

void PanningEffect::processBlockAfter(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
    if (wet > 0) {
        juce::dsp::AudioBlock<float> block (buffer);
        panner.process(juce::dsp::ProcessContextReplacing<float>(block));
    }
};
