#include "DelayEffect.h"
#include "../../../SpoolProcessor.h"


DelayEffect::DelayEffect(SpoolProcessor* processor, int index, int track, int sample) : Effect{ processor, index, track, sample }, delayLine(48000) {
}

DelayEffect::~DelayEffect() {
}


void DelayEffect::prepareToPlay(double sampleRate, int samplesPerBlock) {
    this->sampleRate = sampleRate;
    delayLine.prepare({ sampleRate, (juce::uint32) samplesPerBlock, 2 });
    delayTimeInterpolation.reset(sampleRate, 1);
}

void DelayEffect::processBlockAfter(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
    if (sampleRate < 0) return;

    auto buffRead = buffer.getArrayOfReadPointers();
    auto buffWrite = buffer.getArrayOfWritePointers();

    for (int sampleIndex = 0; sampleIndex < buffer.getNumSamples(); sampleIndex++) {
        float delayTime = delayTimeInterpolation.getNextValue();
        for (int channel = 0; channel < buffer.getNumChannels(); channel++) {
            float drySample = buffRead[channel][sampleIndex];
            delayLine.pushSample(channel, drySample + lastOutput[channel]);

            float delaySample = delayLine.popSample(channel, delayTime, true);
            buffWrite[channel][sampleIndex] += delaySample * wet.get();

            lastOutput[channel] = delaySample * paramTwo.get();
        }
    }
}

void DelayEffect::onParamOneChanged() {
    float targetDelayTime = minDelayTime + (maxDelayTime - minDelayTime) * paramOne.get();
    delayTimeInterpolation.setTargetValue(targetDelayTime);
}
