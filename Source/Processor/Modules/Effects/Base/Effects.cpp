#include <JuceHeader.h>
#include "Effects.h"

#include "Config.h"
#include "Processor/SpoolProcessor.h"
#include "Processor/Modules/Effects/Base/Effect.h"

// actual effects
#include "Processor/Modules/Effects/DelayEffect.h"
#include "Processor/Modules/Effects/ReverbEffect.h"

Effects::Effects(SpoolProcessor* processor, int track, int sample): processor(processor), track(track), sample(sample) {
    addEffect<DelayEffect>();
    addEffect<ReverbEffect>();
    addEffect<DelayEffect>();
    addEffect<DelayEffect>();
    addEffect<DelayEffect>();
    addEffect<DelayEffect>();
    addEffect<DelayEffect>();
    addEffect<DelayEffect>();
    addEffect<DelayEffect>();
    addEffect<DelayEffect>();
    addEffect<DelayEffect>();
    addEffect<DelayEffect>();
    addEffect<DelayEffect>();
    addEffect<DelayEffect>();
    addEffect<DelayEffect>();
    addEffect<DelayEffect>();
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
