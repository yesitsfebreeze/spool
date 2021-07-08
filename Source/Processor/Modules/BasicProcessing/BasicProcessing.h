#pragma once

#include <JuceHeader.h>
#include "Processor/Modules/Parameters/ParameterValue.h"

#include "Processor/Modules/BasicProcessing/Processors/VolumeProcessor.h"
#include "Processor/Modules/BasicProcessing/Processors/BalanceProcessor.h"


class BasicProcessing {

    std::unique_ptr<juce::AudioBuffer<float>> buffer;
    std::unique_ptr<juce::AudioBuffer<float>> mergeBuffer;
    
    std::unique_ptr<ParameterValue> volume;
    std::unique_ptr<ParameterValue> balance;
    
    std::unique_ptr<ParameterValue> lowqEQ;
    std::unique_ptr<ParameterValue> midEQ;
    std::unique_ptr<ParameterValue> highEQ;
    
    std::unique_ptr<ParameterValue> lowPassFilter;
    std::unique_ptr<ParameterValue> bandPassFilter;
    std::unique_ptr<ParameterValue> highPassFilter;
    
    VolumeProcessor volumeProcessor;
    BalanceProcessor balanceProcessor;
    
    void releasePointers() {
        buffer.release();
        mergeBuffer.release();
        volume.release();
        balance.release();
        lowqEQ.release();
        midEQ.release();
        highEQ.release();
        lowPassFilter.release();
        bandPassFilter.release();
        highPassFilter.release();
    }
    
public:
    
    BasicProcessing() {
        releasePointers();
    }
    
    ~BasicProcessing() {
        releasePointers();
    }

    
    void process() {
        juce::AudioBuffer<float>* buff = buffer.get();
        
        float** buffRead = (float**) buff->getArrayOfReadPointers();
        float** buffWrite = (float**) buff->getArrayOfWritePointers();
        int numSamples = buff->getNumSamples();

        for (int sampleIndex = 0; sampleIndex < numSamples; sampleIndex++) {
            if (volume.get()) volumeProcessor.process(buffRead, buffWrite, sampleIndex, volume.get());
            if (balance.get()) balanceProcessor.process(buffRead, buffWrite, sampleIndex, balance.get());
//            if (eq.get()) balanceProcessor.process();
//            if (filterr.get()) balanceProcessor.process();

        }
        
        if (mergeBuffer.get()) {
            mergeBuffer.get()->addFrom(Config::CHLeft, 0, *buffer, Config::CHLeft, 0, numSamples);
            mergeBuffer.get()->addFrom(Config::CHRight, 0, *buffer, Config::CHRight, 0, numSamples);
        }
    }
    
    BasicProcessing& setBuffer(juce::AudioBuffer<float>& buffer) {
        this->buffer.release();
        this->buffer.reset(&buffer);
        return *this;
    }
    
    BasicProcessing& setBalance(ParameterValue& value) {
        this->balance.release();
        this->balance.reset(&value);
        return *this;
    }
    
    BasicProcessing& setVolume(ParameterValue& value) {
        this->volume.release();
        this->volume.reset(&value);
        return *this;
    }
    
    BasicProcessing& addTo(juce::AudioBuffer<float>& buffer) {
        this->mergeBuffer.release();
        this->mergeBuffer.reset(&buffer);
        return *this;
    }

};
