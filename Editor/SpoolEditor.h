#pragma once

#include <JuceHeader.h>
#include "../Processor/SpoolProcessor.h"
#include "CommandDefinitions.h"

//==============================================================================
/**
*/
class SpoolEditor : public juce::AudioProcessorEditor, public juce::ApplicationCommandTarget {
public:
    SpoolEditor (SpoolProcessor&);
    ~SpoolEditor() override;
    
    ApplicationCommandTarget* getNextCommandTarget() override;
    void getAllCommands (juce::Array<juce::CommandID>& commands) override;
    void getCommandInfo (juce::CommandID commandID, juce::ApplicationCommandInfo& result) override;
    bool perform (const InvocationInfo& info) override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    juce::ApplicationCommandManager commandManager;
    CommandDefinitions commandDefinitions;
    SpoolProcessor& audioProcessor;

private:
    void timerCallback(bool isBeat, bool isUpBeat);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpoolEditor)
};
