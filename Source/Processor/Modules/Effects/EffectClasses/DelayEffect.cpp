#include "DelayEffect.h"
#include "../../../SpoolProcessor.h"


DelayEffect::DelayEffect(SpoolProcessor* processor, int index, int track, int sample) : Effect{ processor, index, track, sample } {
}

DelayEffect::~DelayEffect() {
}


void DelayEffect::prepareToPlay(double sampleRate, int samplesPerBlock) {
    
};

void DelayEffect::processBlockAfter(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
    
};
