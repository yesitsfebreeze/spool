#include "../Processor/SpoolProcessor.h"
#include "GUI.h"
#include "../Config.h"
#include "UI/Base/UI.h"

//==============================================================================
GUI::GUI (SpoolProcessor& p) : AudioProcessorEditor (&p), audioProcessor (p) {
    ui.reset(new UI());
    setSize(GUIConfig::DefaultWindowSize, GUIConfig::DefaultWindowSize);
    
    // commandmanager
    commandManager.registerAllCommandsForTarget(this);
    getTopLevelComponent()->addKeyListener(commandManager.getKeyMappings());
    setWantsKeyboardFocus(true);
    juce::Timer::callAfterDelay (300, [this] { grabKeyboardFocus(); });
    
    audioProcessor.guiTimerCallback = [this] (bool isBeat, bool isUpBeat) {timerCallback(isBeat,isUpBeat);};
    
    addAndMakeVisible(ui.get());
    
    ui->setReferences(&audioProcessor, this);
}

GUI::~GUI() {
    ui.reset();
}

void GUI::timerCallback(bool isBeat, bool isUpBeat) {

}


juce::ApplicationCommandTarget* GUI::getNextCommandTarget() {
    return nullptr;
}

void GUI::getAllCommands(juce::Array<juce::CommandID>& commands) {
    commandDefinitions.getAllCommands(commands);
}

void GUI::getCommandInfo(juce::CommandID commandID, juce::ApplicationCommandInfo& result) {
    commandDefinitions.getCommandInfo(commandID, result);
}

bool GUI::perform(const InvocationInfo& info) {
    bool isLatching = info.commandFlags == CommandDefinitions::CustomCommandFlags::isLatching;
    bool executed = audioProcessor.commandQueue.invoke((Config::Command::ID) info.commandID, info.isKeyDown, isLatching);
    if (!executed) DBG("Command is not implemented yet");
    return true;
}

void GUI::executeCommand(Config::Command::ID commandID, bool isKeyDown, bool isLatching) {
    commandInfo.reset(new InvocationInfo(commandID));
    commandInfo->isKeyDown = isKeyDown;
    if (isLatching) commandInfo->commandFlags = CommandDefinitions::CustomCommandFlags::isLatching;
    
    // This call isn't thread-safe for use from a non-UI thread without locking the message
    // manager first..
    JUCE_ASSERT_MESSAGE_MANAGER_IS_LOCKED

    bool ok = false;
    juce::ApplicationCommandInfo info (0);

    if (auto* target = commandManager.getTargetForCommand (commandInfo->commandID, info)) {
        ok = target->invoke (*commandInfo.get(), false);
        commandManager.commandStatusChanged();
    }

    commandInfo.reset();
}

//==============================================================================
void GUI::paint (juce::Graphics& g) {
    juce::Rectangle<float> bounds = getLocalBounds().toFloat();
    ui->setBounds(0, 0, bounds.getWidth(), bounds.getHeight());
}

void GUI::resized() {
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your gui..
}
