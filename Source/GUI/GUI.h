#pragma once

#include <JuceHeader.h>
#include "Processor/SpoolProcessor.h"
#include "GUI/CommandDefinitions.h"

class UI;

class GUI : public juce::AudioProcessorEditor, public juce::ApplicationCommandTarget {

public:
    using InvocationInfo = juce::ApplicationCommandTarget::InvocationInfo;

    GUI (SpoolProcessor&);
    ~GUI() override;
    
    ApplicationCommandTarget* getNextCommandTarget() override;
    void getAllCommands (juce::Array<juce::CommandID>& commands) override;
    void getCommandInfo (juce::CommandID commandID, juce::ApplicationCommandInfo& result) override;
    bool perform (const InvocationInfo& info) override;
    void executeCommand(Config::Command::ID commandID, bool isKeyDown = false, bool isLatching = false);

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    juce::ApplicationCommandManager commandManager;
    CommandDefinitions commandDefinitions;
    SpoolProcessor& audioProcessor;

private:
    std::unique_ptr<UI> ui;
    void timerCallback(bool isBeat, bool isUpBeat);
    std::unique_ptr<InvocationInfo> commandInfo;
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GUI)
};
