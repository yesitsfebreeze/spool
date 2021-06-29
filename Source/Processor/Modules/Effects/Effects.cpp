#include <JuceHeader.h>
#include "../../../Config.h"
#include "Effects.h"
#include "Effect.h"
#include "EffectList.h"
#include "../../SpoolProcessor.h"

Effects::Effects(SpoolProcessor* processor, int track, int sample): processor(processor), track(track), sample(sample) {
    effects.add(new DelayEffect(processor, effects.size(), track, sample));
    effects.add(new PanningEffect(processor, effects.size(), track, sample));
}

Effects::~Effects() {
    
}


void Effects::prepareToPlay(double sampleRate, int samplesPerBlock) {
    for (Effect* effect : effects) {
        effect->prepareToPlay(sampleRate, samplesPerBlock);
    }
}

void Effects::processBlockBefore(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
    for (Effect* effect : effects) {
        effect->processBlockBefore(buffer, midiMessages);
    }
};

void Effects::processBlockAfter(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
    for (Effect* effect : effects) {
        effect->processBlockAfter(buffer, midiMessages);
    }
};
