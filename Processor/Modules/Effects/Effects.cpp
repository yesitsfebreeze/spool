#include <JuceHeader.h>
#include "../../../Config.h"
#include "Effects.h"
#include "FXBase.h"
#include "FXList.h"

Effects::Effects(int track, int sample): track(track), sample(sample) {
    effects.add(new DelayFX(0, track, sample));
}

Effects::~Effects() {
    
}


void Effects::processBlockBefore(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
    for (FXBase* effect : effects) {
        effect->processBlockBefore(buffer, midiMessages);
    }
};

void Effects::processBlockAfter(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
    for (FXBase* effect : effects) {
        effect->processBlockAfter(buffer, midiMessages);
    }
};
