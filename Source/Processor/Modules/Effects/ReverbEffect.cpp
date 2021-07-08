/*
  ==============================================================================

    ReverbEffect.cpp
    Created: 7 Jul 2021 7:53:14pm
    Author:  ryand

  ==============================================================================
*/

#include "ReverbEffect.h"

ReverbEffect::ReverbEffect(SpoolProcessor* processor, ParameterValue& wet, ParameterValue& paramA, ParameterValue& paramB, int index, int track, int sample) :
    Effect{ processor, wet, paramA, paramB, index, track, sample }
{
}

ReverbEffect::~ReverbEffect()
{
}

void ReverbEffect::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    reverb.setSampleRate(sampleRate);
}

void ReverbEffect::processBlockAfter(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{

    /* Juce wants to handle dry/wet mixing with this particular processor via reverbParameters */
    if (buffer.getNumChannels() == 1)
    {
        reverb.processMono(buffer.getWritePointer(0), buffer.getNumSamples());
    }
    else
    {
        reverb.processStereo(buffer.getWritePointer(0), buffer.getWritePointer(1), buffer.getNumSamples());
    }
}

void ReverbEffect::onWetChanged()
{
    reverbParameters.wetLevel = wet.percent;
    reverb.setParameters(reverbParameters);
}

void ReverbEffect::onParamAChanged()
{
    reverbParameters.roomSize = paramA.percent;
    reverb.setParameters(reverbParameters);
    reverb.setParameters(reverbParameters);
}

