#pragma once

#include <JuceHeader.h>
#include "Modules/Tracks/Tracks.h"
#include "Modules/Commands/CommandQueue.h"
#include "Commands.h"
#include "Parameteres.h"


// forward refs
class Sequencer;

class SpoolProcessor  : public juce::AudioProcessor, public juce::Timer
{
public:
    //==============================================================================
    SpoolProcessor();
    ~SpoolProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void timerCallback() override;
    void beatCallback(int beat, bool isUpBeat);
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    void setRecordLength(int length) {
        recordLength = length;
    }

    void changeRecordLength(int value) {
        recordLength += value;
    }
    
    int getRecordLength() {
        return recordLength;
    }
    
    bool setEffectMode(bool state) {
        effectMode = state;
        return effectMode;
    }
    
    bool isEffectMode() {
        return effectMode;
    }

    // vars
    std::function<void(bool isBeat, bool isUpbeat)> editorTimerCallback;
    std::unique_ptr<Sequencer> sequencer;
    std::unique_ptr<Tracks> tracks;
    CommandQueue commandQueue;
    juce::dsp::ProcessSpec processSpec;
    
    bool isFunctionDown = false;
    bool isMuteDown = false;
    bool isPlayDown = false;
    bool isRecordDown = false;

private:
    Commands commands;
    bool effectMode = false;
    juce::int64 loopIndex = 0;
    float timePerUpdate = 1000 / Config::UpdateHz;
    juce::int64 currentTime = 0;
    
    //TODO: set record length via command
    int recordLength = Config::DefaultRecordLength;

    void getCurrentTime() {
        loopIndex++;
        currentTime = timePerUpdate * loopIndex;
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpoolProcessor)
};
