#include "../Processor/SpoolProcessor.h"
#include "SpoolEditor.h"
#include "../Config.h"


//==============================================================================
SpoolEditor::SpoolEditor (SpoolProcessor& p) : AudioProcessorEditor (&p), audioProcessor (p) {
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    
    // commandmanager
    commandManager.registerAllCommandsForTarget(this);
    getTopLevelComponent()->addKeyListener(commandManager.getKeyMappings());
    setWantsKeyboardFocus(true);
    juce::Timer::callAfterDelay (300, [this] { grabKeyboardFocus(); });
    
    audioProcessor.editorTimerCallback = [this] (bool isBeat, bool isUpBeat) {timerCallback(isBeat,isUpBeat);};
}

SpoolEditor::~SpoolEditor() {
}

void SpoolEditor::timerCallback(bool isBeat, bool isUpBeat) {
    
}


juce::ApplicationCommandTarget* SpoolEditor::getNextCommandTarget() {
    return nullptr;
}

void SpoolEditor::getAllCommands (juce::Array<juce::CommandID>& commands) {
    commandDefinitions.getAllCommands(commands);
}

void SpoolEditor::getCommandInfo (juce::CommandID commandID, juce::ApplicationCommandInfo& result) {
    commandDefinitions.getCommandInfo(commandID, result);
}

bool SpoolEditor::perform (const InvocationInfo& info) {
    bool executed = audioProcessor.commandQueue.triggerCommand((Config::Command::ID) info.commandID, info.isKeyDown);
    if (!executed) {
        DBG("Not implemented yet");
    }

    return true;
}

//==============================================================================
void SpoolEditor::paint (juce::Graphics& g) {
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void SpoolEditor::resized() {
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
