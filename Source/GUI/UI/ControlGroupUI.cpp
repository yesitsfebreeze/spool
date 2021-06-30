#include "ControlGroupUI.h"


void ControlGroupUI::initializeKnobs() {
    addVolumeKnob();
    addDryWetKnob();
    addParamOneKnob();
    addParamTwoKnob();
}

void ControlGroupUI::addToGroup() {
    
    if (index == 0) {
        if (processor->isEffectMode()) {
            processor->tracks->doForAllTracks(Track::Action::RemoveEffectFromGroupA);
            processor->tracks->doForSelectedTracks(Track::Action::AddEffectToGroupA);
        } else {
            processor->tracks->doForSelectedTracks(Track::Action::RemoveTrackFromGroupB);
            processor->tracks->doForSelectedTracks(Track::Action::AddTrackToGroupA);
        }
    }
    
    if (index == 1) {
        if (processor->isEffectMode()) {
            processor->tracks->doForAllTracks(Track::Action::RemoveEffectFromGroupB);
            processor->tracks->doForSelectedTracks(Track::Action::AddEffectToGroupB);
        } else {
            processor->tracks->doForSelectedTracks(Track::Action::RemoveTrackFromGroupA);
            processor->tracks->doForSelectedTracks(Track::Action::AddTrackToGroupB);
        }
    }

    processor->tracks->doForAllTracks(Track::Action::Select, Track::ActionMode::Off);
    processor->tracks->doForAllTracks(Track::Action::SelectEffect, Track::ActionMode::Off);
    
    processor->setEffectMode(false);
    processor->isFunctionDown = false;
}


void ControlGroupUI::addVolumeKnob() {
    UIKnobComponent* volumeKnob = knobs.add(new UIKnobComponent());
    volumeKnob->setSensitivity(Config::KnobSensitivity);

    volumeKnob->onInteract = [this] () {
        addToGroup();
    };
        
    volumeKnob->onValueChange = [this] (bool increase) {
        ControlGroup group = nullptr;
        if (index == 0) group = processor->controlGroupA;
        if (index == 1) group = processor->controlGroupB;

        group.doForTracks([this, increase] (Track* track) {
            float value = Parameters::getTrackParam(track->getIndex(), Config::Parameters::Volume);
            if (increase) value += (float) Config::ParamChangePerStep;
            if (!increase) value -= (float) Config::ParamChangePerStep;
            Parameters::setTrackParam(track->getIndex(), Config::Parameters::Volume, value);
        });
    };
    
    volumeKnob->onPress = [this] () {
//        app->addSelectedTracksToGroup(index);
    };

    volumeKnob->onRelease = [this] () {
//        app->onRecordLengthRelease();
    };

    volumeKnob->onAlternatePress = [this] () {
        DBG("onAlternatePress");
    };

    volumeKnob->onAlternateRelease = [this] () {
        DBG("onAlternateRelease");
    };

    volumeKnob->setColor(GUIConfig::Colors::Green);
    addAndMakeVisible(volumeKnob);
}

void ControlGroupUI::addDryWetKnob() {
    UIKnobComponent* dryWetKnob = knobs.add(new UIKnobComponent());
    dryWetKnob->setSensitivity(Config::KnobSensitivity);

    dryWetKnob->onInteract = [this] () {
        addToGroup();
    };

    dryWetKnob->onValueChange = [this] (bool increase) {
        ControlGroup group = nullptr;
        if (index == 0) group = processor->controlGroupA;
        if (index == 1) group = processor->controlGroupB;
        
        group.doForEffects([this, increase] (Track* track, Effect* effect) {
            float value = Parameters::getTrackEffectParam(track->getIndex(), effect->index, Config::Parameters::Wet);
            if (increase) value += (float) Config::ParamChangePerStep;
            if (!increase) value -= (float) Config::ParamChangePerStep;
            Parameters::setTrackEffectParam(track->getIndex(), effect->index, Config::Parameters::Wet, value);
        });
    };

    dryWetKnob->onPress = [this] () {
//        app->addSelectedTracksToGroup(index);
    };

    dryWetKnob->onRelease = [this] () {
//        app->onRecordLengthRelease();
    };

    dryWetKnob->onAlternatePress = [this] () {
        DBG("onAlternatePress");
    };

    dryWetKnob->onAlternateRelease = [this] () {
        DBG("onAlternateRelease");
    };

    dryWetKnob->setColor(groupColor);
    addAndMakeVisible(dryWetKnob);
}

void ControlGroupUI::addParamOneKnob() {
    UIKnobComponent* paramOneKnob = knobs.add(new UIKnobComponent());
    paramOneKnob->setSensitivity(Config::KnobSensitivity);
    
    paramOneKnob->onInteract = [this] () {
        addToGroup();
    };

    paramOneKnob->onValueChange = [this] (bool increase) {
        ControlGroup group = nullptr;
        if (index == 0) group = processor->controlGroupA;
        if (index == 1) group = processor->controlGroupB;
        
        group.doForEffects([this, increase] (Track* track, Effect* effect) {
            float value = Parameters::getTrackEffectParam(track->getIndex(), effect->index, Config::Parameters::ParamOne);
            if (increase) value += (float) Config::ParamChangePerStep;
            if (!increase) value -= (float) Config::ParamChangePerStep;
            Parameters::setTrackEffectParam(track->getIndex(), effect->index, Config::Parameters::ParamOne, value);
        });
    };

    paramOneKnob->onPress = [this] () {
//        app->addSelectedTracksToGroup(index);
    };

    paramOneKnob->onRelease = [this] () {
//        app->onRecordLengthRelease();
    };

    paramOneKnob->onAlternatePress = [this] () {
        DBG("onAlternatePress");
    };

    paramOneKnob->onAlternateRelease = [this] () {
        DBG("onAlternateRelease");
    };

    paramOneKnob->setColor(groupColor);
    addAndMakeVisible(paramOneKnob);
}

void ControlGroupUI::addParamTwoKnob() {
    UIKnobComponent* paramTwoKnob = knobs.add(new UIKnobComponent());
    paramTwoKnob->setSensitivity(Config::KnobSensitivity);
    
    paramTwoKnob->onInteract = [this] () {
        addToGroup();
    };

    paramTwoKnob->onValueChange = [this] (bool increase) {
        ControlGroup group = nullptr;
        if (index == 0) group = processor->controlGroupA;
        if (index == 1) group = processor->controlGroupB;
        
        group.doForEffects([this, increase] (Track* track, Effect* effect) {
            float value = Parameters::getTrackEffectParam(track->getIndex(), effect->index, Config::Parameters::ParamTwo);
            if (increase) value += (float) Config::ParamChangePerStep;
            if (!increase) value -= (float) Config::ParamChangePerStep;
            Parameters::setTrackEffectParam(track->getIndex(), effect->index, Config::Parameters::ParamTwo, value);
        });
    };

    paramTwoKnob->onPress = [this] () {
//        app->addSelectedTracksToGroup(index);
    };

    paramTwoKnob->onRelease = [this] () {
//        app->onRecordLengthRelease();
    };

    paramTwoKnob->onAlternatePress = [this] () {
        DBG("onAlternatePress");
    };

    paramTwoKnob->onAlternateRelease = [this] () {
        DBG("onAlternateRelease");
    };

    paramTwoKnob->setColor(groupColor);
    addAndMakeVisible(paramTwoKnob);
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
        
        knob->setPadding(GUIConfig::Padding * 0.6);
    }
    
}
