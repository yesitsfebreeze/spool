#include "AutoPanEffect.h"
#include "Processor/SpoolProcessor.h"


AutoPanEffect::AutoPanEffect(SpoolProcessor* processor, ParameterValue& wet, ParameterValue& paramA, ParameterValue& paramB, int index, int track, int sample) : Effect{ processor, wet, paramA, paramB, index, track, sample } {
    LFO.initialise([] (float x) { return std::sin(x); }, 128);
    LFO.setFrequency(minFrequency);
    randomLFO.initialise(minFrequency);
}


AutoPanEffect::~AutoPanEffect() {
}


void AutoPanEffect::prepareToPlay(double sampleRate, int samplesPerBlock) {     
    LFO.prepare({processor->processSpec.sampleRate / LFOUpdateRate, processor->processSpec.maximumBlockSize, processor->processSpec.numChannels});
    randomLFO.prepare(processor->processSpec);
}


void AutoPanEffect::processBlockAfter(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
    if (wet.percent == 0) return;
    float panData = LFO.processSample(0.0f) * wet.percent;
    float rndLFOData = 0.0f;

    if (paramB.percent > 0) {
        rndLFOData = ((2 * randomLFO.process()) * paramB.percent) - 1;
        panData *= 1 - paramB.percent;
        panData += rndLFOData;
    }
    
    DBG(panData);
    
    float balanceL = 1.0f;
    float balanceR = 1.0f;
    
    if (panData < 0) balanceR = 1 + panData;
    if (panData > 0) balanceL = 1 - panData;

    float** buffRead = (float**) buffer.getArrayOfReadPointers();
    float** buffWrite = (float**) buffer.getArrayOfWritePointers();

    int numSamples = buffer.getNumSamples();

    for (int sampleIndex = 0; sampleIndex < numSamples; sampleIndex++) {
        float drySampleL = buffRead[Config::CHLeft][sampleIndex];
        float drySampleR = buffRead[Config::CHRight][sampleIndex];
        float tmpSampleL = drySampleL;
        float tmpSampleR = drySampleR;

        tmpSampleL *= balanceL;
        tmpSampleR *= balanceR;

        buffWrite[Config::CHLeft][sampleIndex] = tmpSampleL;
        buffWrite[Config::CHRight][sampleIndex] = tmpSampleR;
    }
};

void AutoPanEffect::onParamAChanged() {
    targetFrequency = minFrequency + (maxFrequency - minFrequency) * paramA.percent;
    LFO.setFrequency(targetFrequency);
    onParamBChanged();
}


void AutoPanEffect::onParamBChanged() {
    randomLFO.setFrequency(targetFrequency);
}
