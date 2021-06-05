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


    // vars
    std::function<void(bool isBeat, bool isUpbeat)> editorTimerCallback;
    Sequencer* sequencer;
    CommandQueue commandQueue;
    Tracks tracks;
    Effects effects;

private:
    Commands commands;
    
    juce::int64 getCurrentTime() {
        return juce::Time::getCurrentTime().toMilliseconds();
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpoolProcessor)
};
