#include "ControlGroupUI.h"


void ControlGroupUI::initializeKnobs() {
    addVolumeKnob();
    addDryWetKnob();
    addParamOneKnob();
    addParamTwoKnob();
}

void ControlGroupUI::addToGroup() {
    if (index == 0) processor->tracks->doForSelectedTracks(TrackAction::AddToGroupOne);
    if (index == 1) processor->tracks->doForSelectedTracks(TrackAction::AddToGroupTwo);
    processor->tracks->doForAllTracks(TrackAction::Select, TrackActionMode::Off);
}


void ControlGroupUI::addVolumeKnob() {
    UIKnobComponent* volumeKnob = knobs.add(new UIKnobComponent());
    volumeKnob->setSensitivity(Config::KnobSensitivity);

    volumeKnob->onInteract = [this] () {
        addToGroup();
    };
        
    volumeKnob->onValueChange = [this] (bool increase) {
        float value = Parameters::getTrackEffectParam(0, 0, Config::Parameters::Wet);
        if (increase) value += Config::ParamChangePerStep;
        if (!increase) value -= Config::ParamChangePerStep;
        Parameters::setTrackEffectParam(0, 0, Config::Parameters::Wet, value);
    };
    
    volumeKnob->onPress = [this] () {
//        app->addSelectedTracksToGroup(index);
    };
    volumeKnob->onRelease = [this] () {
//        app->onRecordLengthRelease();
    };
    volumeKnob->onPressAlternate = [this] () {
        DBG("onPressAlternate");
    };
    volumeKnob->onReleaseAlternate = [this] () {
        DBG("onReleaseAlternate");
    };
    volumeKnob->setColor(EditorConfig::Colors::Green);
    addAndMakeVisible(volumeKnob);
}


void ControlGroupUI::addParamTwoKnob() {
    UIKnobComponent* paramTwoKnob = knobs.add(new UIKnobComponent());
    paramTwoKnob->setSensitivity(Config::KnobSensitivity);
    
    paramTwoKnob->onInteract = [this] () {
        addToGroup();
    };

    paramTwoKnob->onValueChange = [this] (bool increase) {
        float value = Parameters::getTrackEffectParam(0, 0, Config::Parameters::ParamOne);
        if (increase) value += Config::ParamChangePerStep;
        if (!increase) value -= Config::ParamChangePerStep;
        Parameters::setTrackEffectParam(0, 0, Config::Parameters::ParamOne, value);
    };
    paramTwoKnob->onPress = [this] () {
//        app->addSelectedTracksToGroup(index);
    };
    paramTwoKnob->onRelease = [this] () {
//        app->onRecordLengthRelease();
    };
    paramTwoKnob->onPressAlternate = [this] () {
        DBG("onPressAlternate");
    };
    paramTwoKnob->onReleaseAlternate = [this] () {
        DBG("onReleaseAlternate");
    };
    paramTwoKnob->setColor(groupColor);
    addAndMakeVisible(paramTwoKnob);
}


void ControlGroupUI::addParamOneKnob() {
    UIKnobComponent* paramOneKnob = knobs.add(new UIKnobComponent());
    paramOneKnob->setSensitivity(Config::KnobSensitivity);
    
    paramOneKnob->onInteract = [this] () {
        addToGroup();
    };

    paramOneKnob->onValueChange = [this] (bool increase) {
        float value = Parameters::getTrackEffectParam(0, 0, Config::Parameters::ParamTwo);
        if (increase) value += Config::ParamChangePerStep;
        if (!increase) value -= Config::ParamChangePerStep;
        Parameters::setTrackEffectParam(0, 0, Config::Parameters::ParamTwo, value);
    };

    paramOneKnob->onPress = [this] () {
//        app->addSelectedTracksToGroup(index);
    };
    paramOneKnob->onRelease = [this] () {
//        app->onRecordLengthRelease();
    };
    paramOneKnob->onPressAlternate = [this] () {
        DBG("onPressAlternate");
    };
    paramOneKnob->onReleaseAlternate = [this] () {
        DBG("onReleaseAlternate");
    };
    paramOneKnob->setColor(groupColor);
    addAndMakeVisible(paramOneKnob);
}


void ControlGroupUI::addDryWetKnob() {
    UIKnobComponent* dryWetKnob = knobs.add(new UIKnobComponent());
    dryWetKnob->setSensitivity(Config::KnobSensitivity);

    dryWetKnob->onInteract = [this] () {
        addToGroup();
    };

    dryWetKnob->onValueChange = [this] (bool increase) {
//        app->setRecordLength(increase, recordIncrease);
    };
    dryWetKnob->onPress = [this] () {
//        app->addSelectedTracksToGroup(index);
    };
    dryWetKnob->onRelease = [this] () {
//        app->onRecordLengthRelease();
    };
    dryWetKnob->onPressAlternate = [this] () {
        DBG("onPressAlternate");
    };
    dryWetKnob->onReleaseAlternate = [this] () {
        DBG("onReleaseAlternate");
    };
    dryWetKnob->setColor(groupColor);
    addAndMakeVisible(dryWetKnob);
}


void ControlGroupUI::resized() {
    juce::Rectangle<int> bounds = getLocalBounds();
    juce::Point<int> center = bounds.getCentre();

    int w = bounds.getWidth();
    int h = bounds.getHeight();
    
    int knobSize = w * 0.8;
    int spacing = (h - knobSize * knobs.size()) / (knobs.size() - 1);
    int centerX = center.getX() - knobSize / 2;
    
    for (int s = 0; s < knobs.size(); s++) {
        UIKnobComponent* knob = knobs[s];
        if (s == 0) {
            knob->setBounds(centerX, 0, knobSize, knobSize);
        } else {
            knob->setBounds(centerX, s * (knobSize + spacing), knobSize, knobSize);
        }
        
        knob->setPadding(EditorConfig::Padding * 0.6);
    }
    
}
