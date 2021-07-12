#pragma once

#include <JuceHeader.h>

class RandomLFO {
private:
    bool isInitialized = false;
    
    float frequency = 0.3f;
    int updateRate = 128;
    int oscCount = 6;
    
    float sampleRate = 0.f;
    juce::uint32 maximumBlockSize = 0.f;
    juce::uint32 numChannels = 0;

    std::vector<juce::dsp::Oscillator<float>> lfos;

public:
    
    void initialise(float defaultFrequency = 0.3) {
        lfos.clear();

        for (int index = 0; index < oscCount; index++) {
            addOSC(index);
        }
        
        setFrequency(defaultFrequency);
        
        isInitialized = true;
    }

    void prepare(juce::dsp::ProcessSpec spec) {
        sampleRate = spec.sampleRate;
        maximumBlockSize = spec.maximumBlockSize;
        numChannels = spec.numChannels;

        update(true, true);
    }

    float process() {
        float data = 0.0f;
        if (!isInitialized) return data;
        for (int index = 0; index < oscCount; index++) {
            data += lfos[index].processSample(0.0f);
        }
        data = data / oscCount;
        return data;
    }
    
// setters
    void setUpdateRate(int rate) {
        updateRate = rate;
        if (!isInitialized) return;
        update(false, true);
    }
    
    void setOscCount(int count) {
        oscCount = count;
        isInitialized = false;
        initialise();
        update(true, true);
    }
    
    void setFrequency(float freq) {
        frequency = freq;
        if (!isInitialized) return;
        update(true, false);
    }
    
private:
    void update(bool frequencyChanged = false, bool updateRateChanged = false) {
        for (int index = 0; index < oscCount; index++) {
            if (updateRateChanged) lfos[index].prepare({sampleRate / updateRate, maximumBlockSize, numChannels});
            if (frequencyChanged) lfos[index].setFrequency(frequency / 4 * (index + 1));
        }
    }
    
    void addOSC(int index) {
        lfos.push_back(juce::dsp::Oscillator<float>());
        lfos[index].initialise([] (float x) { return x < 0.0f ? 0.0f : 1.0f; });
    }
};
