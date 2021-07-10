#include "AutoPanEffect.h"
#include "Processor/SpoolProcessor.h"


AutoPanEffect::AutoPanEffect(SpoolProcessor* processor, ParameterValue& wet, ParameterValue& paramA, ParameterValue& paramB, int index, int track, int sample) :
    Effect{ processor, wet, paramA, paramB, index, track, sample } {
    lfo.initialise([] (float x) { return std::sin(x); }, 128);
    lfo.setFrequency(minFrequency);
    
//    rndLfoOne.initialise([] (float x) { return std::sin(x); }, 128);
//    rndLfoOne.setFrequency(minFrequency * rndOneSpeed);
//    rndLfoTwo.initialise([] (float x) { return std::sin(x); }, 128);
//    rndLfoTwo.setFrequency(minFrequency * rndTwoSpeed);
//    rndLfoThree.initialise([] (float x) { return std::sin(x); }, 128);
//    rndLfoThree.setFrequency(minFrequency * rndThreeSpeed);
//    rndLfoFour.initialise([] (float x) { return std::sin(x); }, 128);
//    rndLfoFour.setFrequency(minFrequency * rndFourSpeed);
}


AutoPanEffect::~AutoPanEffect() {
}


void AutoPanEffect::prepareToPlay(double sampleRate, int samplesPerBlock) {     
    lfo.prepare({ processor->processSpec.sampleRate / lfoUpdateRate, processor->processSpec.maximumBlockSize, processor->processSpec.numChannels });
//    rndLfoOne.prepare({ processor->processSpec.sampleRate / lfoUpdateRate, processor->processSpec.maximumBlockSize, processor->processSpec.numChannels });
//    rndLfoTwo.prepare({ processor->processSpec.sampleRate / lfoUpdateRate, processor->processSpec.maximumBlockSize, processor->processSpec.numChannels });
//    rndLfoThree.prepare({ processor->processSpec.sampleRate / lfoUpdateRate, processor->processSpec.maximumBlockSize, processor->processSpec.numChannels });
//    rndLfoFour  .prepare({ processor->processSpec.sampleRate / lfoUpdateRate, processor->processSpec.maximumBlockSize, processor->processSpec.numChannels });
}


void AutoPanEffect::processBlockAfter(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
    if (wet.percent == 0) return;
    auto lfoData = lfo.processSample(0.0f) * wet.percent;
    
//    float pseudoRndOne = (rndLfoOne.processSample(0.0f) > 0) ? 1.0f : 0.0f;
//    float pseudoRndTwo = (rndLfoTwo.processSample(0.0f) > 0) ? 1.0f : 0.0f;
//    float pseudoRndThree = (rndLfoThree.processSample(0.0f) > 0) ? 1.0f : 0.0f;
//    float pseudoRndFour = (rndLfoFour.processSample(0.0f) > 0) ? 1.0f : 0.0f;
//    float rndValue = pseudoRndOne - pseudoRndTwo/2 - pseudoRndThree/4 - pseudoRndFour/8;
//    lfoData = lfoData * rndValue;
    
    float balanceL = 1.0f;
    float balanceR = 1.0f;
    
    if (lfoData < 0) balanceR = 1 + lfoData;
    if (lfoData > 0) balanceL = 1 - lfoData;

    float** buffRead = (float**) buffer.getArrayOfReadPointers();
    float** buffWrite = (float**) buffer.getArrayOfWritePointers();

    int numSamples = buffer.getNumSamples();

    for (int sampleIndex = 0; sampleIndex < numSamples; sampleIndex++) {
        float drySampleL = buffRead[Config::CHLeft][sampleIndex];
        float drySampleR = buffRead[Config::CHRight][sampleIndex];
        float tmpSampleL = drySampleL;
        float tmpSampleR = drySampleR;

        tmpSampleL *= balanceL;
        if (balanceR < 1) {
            tmpSampleL *= balanceR;
            tmpSampleL += drySampleR * (1 - balanceR);
        }

        tmpSampleR *= balanceR;
        if (balanceL < 1) {
            tmpSampleR *= balanceL;
            tmpSampleR += drySampleL * (1 - balanceL);
        }

        buffWrite[Config::CHLeft][sampleIndex] = tmpSampleL;
        buffWrite[Config::CHRight][sampleIndex] = tmpSampleR;
    }
};

void AutoPanEffect::onParamAChanged() {
    float targetFrequency = minFrequency + (maxFrequency - minFrequency) * paramA.percent;
    lfo.setFrequency(targetFrequency);
}
