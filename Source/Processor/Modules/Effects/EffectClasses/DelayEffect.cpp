#include "DelayEffect.h"
#include "../../../SpoolProcessor.h"


DelayEffect::DelayEffect(SpoolProcessor* processor, int index, int track, int sample) : BaseEffect{ processor, index, track, sample }, 
delayLine(48000) 
{

}

DelayEffect::~DelayEffect() {
}


void DelayEffect::prepareToPlay(double sampleRate, int samplesPerBlock) {
    mSampleRate = sampleRate;
    delayLine.prepare({ mSampleRate, (uint32)samplesPerBlock, 2 });
    delaySmoothed.reset(mSampleRate, 1);
}

void DelayEffect::processBlockBefore(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
    
}

void DelayEffect::processBlockAfter(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
    auto buffRead = buffer.getArrayOfReadPointers();
    auto buffWrite = buffer.getArrayOfWritePointers();

    for (int sampleIndex = 0; sampleIndex < buffer.getNumSamples(); sampleIndex++)
    {
        delayTime = delaySmoothed.getNextValue();
        for (int channel = 0; channel < buffer.getNumChannels(); channel++)
        {
            float drySample = buffRead[channel][sampleIndex];
            delayLine.pushSample(channel, drySample + lastOutput[channel]);

            float delaySample = delayLine.popSample(channel, delayTime.get(), true);
            delaySample * wetMix.get();
            buffWrite[channel][sampleIndex] += delaySample;

            lastOutput[channel] = delaySample * feedback.get();
        }
    }
}


void DelayEffect::onWetChanged()
{
    wetMix = wet;
}

void DelayEffect::onParamOneChanged(){
    auto dTime = mSampleRate * paramOne;
    delaySmoothed.setTargetValue(dTime);
}

void DelayEffect::onParamTwoChanged(){
    feedback = paramTwo;
}
