#include "TracksUI.h"
#include "../../Config.h"

TracksUI::TracksUI() {
    for (int i = 0; i < Config::Tracks::count; ++i) {
        TrackButtonUI* button = new TrackButtonUI(i);

        int firstTrackCommandEnum = (int) Cmd::Track1;
        Cmd currentTrackCommandEnum = static_cast<Cmd> (firstTrackCommandEnum + i);
        
        button->isSwitch = true;
        button->onPress = [this, button, currentTrackCommandEnum] () {
            editor->executeCommand(currentTrackCommandEnum, true);
        };
        button->onRelease = [this, button, currentTrackCommandEnum] () {
            editor->executeCommand(currentTrackCommandEnum, false);
        };
        addAndMakeVisible(buttons.add(button));
    }
}

void TracksUI::onSetReferences() {
    int buttonSize = buttons.size();
    for (int i = 0; i < buttonSize; ++i) {
        buttons[i]->setReferences(processor, editor);
        buttons[i]->track = processor->tracks.getTrack(i);
    }
}

void TracksUI::resized() {
    int buttonSize = buttons.size();
    for (int i = 0; i < buttonSize; ++i) {
        buttons[i]->calculateBounds(getLocalBounds(), i, Config::Tracks::columns, Config::Tracks::rows);
    }
}


void TrackButtonUI::setGroupColor(juce::Colour borderColor, juce::Colour fillColor) {
    hasGroupColor = true;
    groupBorderColor = borderColor;
    groupFillColor = fillColor;
}

void TrackButtonUI::removeGroupColor() {
    hasGroupColor = false;
    groupBorderColor = originalBorderColor;
    groupFillColor = originalFillColor;
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
    isDepressed = track->isPressed() || track->isSelected();
    
    if (isColorForced) return;

    if (isRecording) {
       borderColor = Config::Colors::red;
       fillColor = Config::Colors::red;
    } else if (isMuted) {
        borderColor = Config::Colors::yellow;
        fillColor = Config::Colors::yellow;
    } else if (isPlaying && !hasGroupColor) {
        borderColor = Config::Colors::green;
        fillColor = Config::Colors::green;
    } else {
        if (hasGroupColor) {
            borderColor = groupBorderColor;
            fillColor = groupFillColor;
        } else {
            borderColor = originalBorderColor;
            fillColor = originalFillColor;
        }
    }
   
    if (!isDepressed && !isRecording) {
        fillColor = Config::Colors::dark;
    }
}

void TrackButtonUI::afterPaint() {

//    if (track->sampleSize == 0) return;
//    if (track->percentPlayed == 0) return;
//
//    if (track->isSelected) {
//        graphics->setColour(Config::Colors::dark);
//    } else {
//        graphics->setColour(Config::Colors::mid);
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


