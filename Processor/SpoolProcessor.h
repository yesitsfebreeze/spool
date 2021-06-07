#pragma once

#include <JuceHeader.h>
#include "Modules/Tracks/Tracks.h"
#include "Modules/Effects.h"
#include "Modules/CommandQueue.h"
#include "Commands.h"


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
    void beatCallback(bool isUpBeat);
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

    
    int getRecordLength() {
        return 5;
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
    Effects effects;
    
    bool isFunctionDown = false;
    bool isMuteDown = false;
    bool isPlayDown = false;
    bool isRecordDown = false;

private:
    Commands commands;
    bool effectMode = false;
    
    juce::int64 getCurrentTime() {
        return juce::Time::getCurrentTime().toMilliseconds();
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpoolProcessor)
};
