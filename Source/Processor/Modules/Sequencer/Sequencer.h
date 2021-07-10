#pragma once

#include <JuceHeader.h>
#include "Config.h"
#include "Processor/SpoolProcessor.h"

class Sequencer {
public:
    Sequencer(SpoolProcessor* owner) : owner(owner) {
        
    }
    
    bool isRunning = false;
    int tick { 0 };
    int beat { 0 };

    void update();
    void setBpm(double bpm);
    double getBpm() {
        return bpm;
    };
    void prepareToPlay(double sampleRate, int samplesPerBlock);
    void processBlockBefore(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);
    void processBlockAfter(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);
    void stop();
    void pause();
    
private:
    SpoolProcessor*     owner;
    double sampleRate   { 0 };
    int totalSamples    { 0 };
    int interval        { 0 };
    double bpm          { Config::DefaultPBM };
};
