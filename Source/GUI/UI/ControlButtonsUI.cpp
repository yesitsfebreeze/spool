#include "ControlButtonsUI.h"
#include "../../Config.h"

ControlButtonsUI::ControlButtonsUI() {
    setFramesPerSecond(GUIConfig::FPS);

    // function button
    functionButton = buttons.add(new UIButtonComponent(0));
    functionButton->setColors(GUIConfig::Colors::Blue, GUIConfig::Colors::Blue);
    functionButton->onPress = [this] () { gui->executeCommand(Config::Command::ID::Function, true); };
    functionButton->onRelease = [this] () { gui->executeCommand(Config::Command::ID::Function, false); };
    // right click latch
    functionButton->onAlternatePress = [this] () {
        gui->executeCommand(Config::Command::ID::Function, true, true);
    };
    addAndMakeVisible(functionButton);
    
    // mute button
    muteButton = buttons.add(new UIButtonComponent(1));
    muteButton->setColors(GUIConfig::Colors::Yellow, GUIConfig::Colors::Yellow);
    muteButton->onPress = [this] () { gui->executeCommand(Config::Command::ID::Mute, true); };
    muteButton->onRelease = [this] () { gui->executeCommand(Config::Command::ID::Mute, false); };
    addAndMakeVisible(muteButton);
    
    // play button
    playButton = buttons.add(new UIButtonComponent(2));
    playButton->setColors(GUIConfig::Colors::Green, GUIConfig::Colors::Green);
    playButton->onPress = [this] () { gui->executeCommand(Config::Command::ID::Play, true); };
    playButton->onRelease = [this] () { gui->executeCommand(Config::Command::ID::Play, false); };
    addAndMakeVisible(playButton);
    
    // record button
    recordButton = buttons.add(new UIButtonComponent(3));
    recordButton->setColors(GUIConfig::Colors::Red, GUIConfig::Colors::Red);
    recordButton->onPress = [this] () { gui->executeCommand(Config::Command::ID::Record, true); };
    recordButton->onRelease = [this] () { gui->executeCommand(Config::Command::ID::Record, false); };
    addAndMakeVisible(recordButton);
}

ControlButtonsUI::~ControlButtonsUI() {

}

void ControlButtonsUI::onSetReferences() {
    int buttonCount = buttons.size();
    for (int i = 0; i < buttonCount; ++i) {
        buttons[i]->setReferences(processor, gui);
    }
}

void ControlButtonsUI::update() {
    functionButton->isDepressed = processor->isFunctionDown;
    muteButton->isDepressed = processor->isMuteDown;
    playButton->isDepressed = processor->isPlayDown;
    recordButton->isDepressed = processor->isRecordDown;
}

void ControlButtonsUI::resized() {
    int buttonCount = buttons.size();
    for (int i = 0; i < buttonCount; ++i) {
        buttons[i]->calculateBounds(getLocalBounds(), i, buttonCount, 1);
    }
}
