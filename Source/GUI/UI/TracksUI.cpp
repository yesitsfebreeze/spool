#include "TracksUI.h"
#include "Config.h"
#include "GUI/GUIConfig.h"

TracksUI::TracksUI() {
    for (int i = 0; i < Config::TrackCount; ++i) {
        TrackButtonUI* button = new TrackButtonUI(i);

        int firstTrackCommandEnum = (int) Cmd::Track1;
        Cmd currentTrackCommandEnum = static_cast<Cmd> (firstTrackCommandEnum + i);
        
        button->isSwitch = true;
        button->onPress = [this, button, currentTrackCommandEnum] () {
            gui->executeCommand(currentTrackCommandEnum, true);
        };
        button->onRelease = [this, button, currentTrackCommandEnum] () {
            gui->executeCommand(currentTrackCommandEnum, false);
        };
        addAndMakeVisible(buttons.add(button));
    }
}

void TracksUI::onSetReferences() {
    int buttonSize = buttons.size();
    for (int i = 0; i < buttonSize; ++i) {
        buttons[i]->setReferences(processor, gui);
        buttons[i]->track = processor->tracks->getTrack(i);
    }
}

void TracksUI::resized() {
    int buttonSize = buttons.size();
    for (int i = 0; i < buttonSize; ++i) {
        buttons[i]->calculateBounds(getLocalBounds(), i, GUIConfig::TrackColumns, GUIConfig::TrackRows);
    }
}

void TrackButtonUI::forceColor(juce::Colour borderColor, juce::Colour fillColor) {
    isColorForced = true;
    this->borderColor = borderColor;
    this->fillColor = fillColor;
}

void TrackButtonUI::removeForcedColor() {
    isColorForced = false;
}

void TrackButtonUI::getButtonColors() {
    if (isColorForced) return;
    
    if (processor->isEffectMode()) {
        isDepressed =  track->isPressed() || track->isEffectSelected();

        borderColor = GUIConfig::Colors::Blue;
        fillColor = GUIConfig::Colors::Dark;
        if (isDepressed) {
            fillColor = GUIConfig::Colors::Blue;
        }
        
        return;
    }

    isDepressed = track->isPressed() || track->isSelected();

    if (isRecording) {
       borderColor = GUIConfig::Colors::Red;
       fillColor = GUIConfig::Colors::Red;
    } else if (track->isMuted()) {
        borderColor = GUIConfig::Colors::Yellow;
        fillColor = GUIConfig::Colors::Yellow;
    } else if (isPlaying && !track->isGrouped()) {
        borderColor = GUIConfig::Colors::Green;
        fillColor = GUIConfig::Colors::Green;
    } else {
        if (track->isGrouped()) {
            if (track->isInGroup(ControlGroup::Group::A)) {
                borderColor = GUIConfig::Colors::GroupColorA;
                fillColor = GUIConfig::Colors::GroupColorA;
            } else if (track->isInGroup(ControlGroup::Group::B)) {
                borderColor = GUIConfig::Colors::GroupColorB;
                fillColor = GUIConfig::Colors::GroupColorB;
            }
        } else {
            borderColor = originalBorderColor;
            fillColor = originalFillColor;
        }
    }

    if (!isDepressed && !isRecording) {
        fillColor = GUIConfig::Colors::Dark;
    }
}

void TrackButtonUI::afterPaint() {

//    if (track->sampleSize == 0) return;
//    if (track->percentPlayed == 0) return;
//
//    if (track->isSelected) {
//        graphics->setColour(GUIConfig::Colors::dark);
//    } else {
//        graphics->setColour(GUIConfig::Colors::mid);
//    }

    
//    float inset = 0.15;
//    int wInset = buttonWidth * inset;
//    int hInset = buttonHeight * inset;
//    int width = static_cast<int>((buttonWidth - wInset * 2) * track->percentPlayed);
//
//    int radius = Config::borderRadius / 2;
//    if (width < radius * 2) {
//        radius = width / 2;
//    }
//
//    graphics->fillRoundedRectangle(buttonX + wInset, buttonY + hInset, width , buttonHeight - hInset * 2, radius);
}


