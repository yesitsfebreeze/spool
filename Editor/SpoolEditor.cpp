#include "../Processor/SpoolProcessor.h"
#include "SpoolEditor.h"
#include "../Config.h"
#include "UI/Base/UI.h"

//==============================================================================
SpoolEditor::SpoolEditor (SpoolProcessor& p) : AudioProcessorEditor (&p), audioProcessor (p) {
    ui.reset(new UI());
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(Config::defaultWindowSize, Config::defaultWindowSize);
    
    // commandmanager
    commandManager.registerAllCommandsForTarget(this);
    getTopLevelComponent()->addKeyListener(commandManager.getKeyMappings());
    setWantsKeyboardFocus(true);
    juce::Timer::callAfterDelay (300, [this] { grabKeyboardFocus(); });
    
    audioProcessor.editorTimerCallback = [this] (bool isBeat, bool isUpBeat) {timerCallback(isBeat,isUpBeat);};
    
    addAndMakeVisible(ui.get());
    
    ui->setReferences(&audioProcessor, this);
}

SpoolEditor::~SpoolEditor() {
    ui.reset();
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

void SpoolEditor::executeCommand(Config::Command::ID commandID, bool isKeyDown) {
    juce::ApplicationCommandTarget::InvocationInfo* info = new juce::ApplicationCommandTarget::InvocationInfo(commandID);
    info->isKeyDown = isKeyDown;
    commandManager.invoke(*info, true);
}

//==============================================================================
void SpoolEditor::paint (juce::Graphics& g) {
    juce::Rectangle<float> bounds = getLocalBounds().toFloat();
    ui->setBounds(0, 0, bounds.getWidth(), bounds.getHeight());
}

void SpoolEditor::resized() {
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
