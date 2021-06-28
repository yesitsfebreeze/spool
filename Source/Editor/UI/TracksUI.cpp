#include "TracksUI.h"
#include "../../Config.h"
#include "../EditorConfig.h"

TracksUI::TracksUI() {
    for (int i = 0; i < Config::TrackCount; ++i) {
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
        buttons[i]->track = processor->tracks->getTrack(i);
    }
}

void TracksUI::resized() {
    int buttonSize = buttons.size();
    for (int i = 0; i < buttonSize; ++i) {
        buttons[i]->calculateBounds(getLocalBounds(), i, EditorConfig::TrackColumns, EditorConfig::TrackRows);
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

    if (processor->isEffectMode()) {
        borderColor = EditorConfig::Colors::Blue;
        fillColor = EditorConfig::Colors::Blue;
    } else if (isRecording) {
       borderColor = EditorConfig::Colors::Red;
       fillColor = EditorConfig::Colors::Red;
    } else if (track->isMuted()) {
        borderColor = EditorConfig::Colors::Yellow;
        fillColor = EditorConfig::Colors::Yellow;
    } else if (isPlaying && !hasGroupColor) {
        borderColor = EditorConfig::Colors::Green;
        fillColor = EditorConfig::Colors::Green;
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
        fillColor = EditorConfig::Colors::Dark;
    }
}

void TrackButtonUI::afterPaint() {

//    if (track->sampleSize == 0) return;
//    if (track->percentPlayed == 0) return;
//
//    if (track->isSelected) {
//        graphics->setColour(EditorConfig::Colors::dark);
//    } else {
//        graphics->setColour(EditorConfig::Colors::mid);
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


