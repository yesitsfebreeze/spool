#include "ControlGroupUI.h"

#include "Processor/Modules/Commands/CommandQueue.h"


void ControlGroupUI::initializeKnobs() {
    addVolumeKnob();
    addDryWetKnob();
    addParamAKnob();
    addParamBKnob();
}

void ControlGroupUI::addVolumeKnob() {
    UIKnobComponent* volumeKnob = knobs.add(new UIKnobComponent());
    volumeKnob->setSensitivity(Config::KnobSensitivity);

    volumeKnob->onInteract = [this] () {
        processor->commandQueue.invokeInstantly(Config::Command::Volume, Config::Command::Action::Interact, groupID);
    };
        
    volumeKnob->onValueChange = [this] (bool increase) {
        if (increase) {
            processor->commandQueue.invokeInstantly(Config::Command::Volume, Config::Command::Action::Increase, groupID);
        } else {
            processor->commandQueue.invokeInstantly(Config::Command::Volume, Config::Command::Action::Decrease, groupID);
        }
    };
    
    volumeKnob->onPress = [this] () {
        processor->commandQueue.invoke(Config::Command::Volume, true);
    };

    volumeKnob->onRelease = [this] () {
        processor->commandQueue.invoke(Config::Command::Volume, false);
    };

    volumeKnob->onAlternatePress = [this] () {
//        DBG("onAlternatePress");
    };

    volumeKnob->onAlternateRelease = [this] () {
//        DBG("onAlternateRelease");
    };

    volumeKnob->setColor(GUIConfig::Colors::Green);
    addAndMakeVisible(volumeKnob);
}

void ControlGroupUI::addDryWetKnob() {
    UIKnobComponent* dryWetKnob = knobs.add(new UIKnobComponent());
    dryWetKnob->setSensitivity(Config::KnobSensitivity);

    dryWetKnob->onInteract = [this] () {
        processor->commandQueue.invokeInstantly(Config::Command::Wet, Config::Command::Action::Interact, groupID);
    };

    dryWetKnob->onValueChange = [this] (bool increase) {
        if (increase) {
            processor->commandQueue.invokeInstantly(Config::Command::Wet, Config::Command::Action::Increase, groupID);
        } else {
            processor->commandQueue.invokeInstantly(Config::Command::Wet, Config::Command::Action::Decrease, groupID);
        }
//        ControlGroup group = nullptr;
//        if (group == ControlGroup::Group::A) group = processor->controlGroupA;
//        if (group == ControlGroup::Group::B) group = processor->controlGroupB;
//
//        group.doForEffects([this, increase] (Track* track, Effect* effect) {
//            float value = Parameters::getTrackEffectParam(track->getIndex(), effect->index, Config::Parameter::Wet);
//            if (increase) value += (float) Config::ParamChangePerStep;
//            if (!increase) value -= (float) Config::ParamChangePerStep;
//            Parameters::setTrackEffectParam(track->getIndex(), effect->index, Config::Parameter::Wet, value);
//        });
    };

    dryWetKnob->onPress = [this] () {
        processor->commandQueue.invoke(Config::Command::Wet, true);
//        app->addSelectedTracksToGroup(index);
    };

    dryWetKnob->onRelease = [this] () {
        processor->commandQueue.invoke(Config::Command::Wet, false);
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

void ControlGroupUI::addParamAKnob() {
    UIKnobComponent* paramAKnob = knobs.add(new UIKnobComponent());
    paramAKnob->setSensitivity(Config::KnobSensitivity);
    
    paramAKnob->onInteract = [this] () {
        processor->commandQueue.invokeInstantly(Config::Command::ParamA, Config::Command::Action::Interact, groupID);
    };

    paramAKnob->onValueChange = [this] (bool increase) {
        if (increase) {
            processor->commandQueue.invokeInstantly(Config::Command::ParamA, Config::Command::Action::Increase, groupID);
        } else {
            processor->commandQueue.invokeInstantly(Config::Command::ParamA, Config::Command::Action::Decrease, groupID);
        }
    };

    paramAKnob->onPress = [this] () {
//        app->addSelectedTracksToGroup(index);
    };

    paramAKnob->onRelease = [this] () {
//        app->onRecordLengthRelease();
    };

    paramAKnob->onAlternatePress = [this] () {
        DBG("onAlternatePress");
    };

    paramAKnob->onAlternateRelease = [this] () {
        DBG("onAlternateRelease");
    };

    paramAKnob->setColor(groupColor);
    addAndMakeVisible(paramAKnob);
}

void ControlGroupUI::addParamBKnob() {
    UIKnobComponent* paramBKnob = knobs.add(new UIKnobComponent());
    paramBKnob->setSensitivity(Config::KnobSensitivity);
    
    paramBKnob->onInteract = [this] () {
        processor->commandQueue.invokeInstantly(Config::Command::ParamB, Config::Command::Action::Interact, groupID);
    };

    paramBKnob->onValueChange = [this] (bool increase) {
        if (increase) {
            processor->commandQueue.invokeInstantly(Config::Command::ParamB, Config::Command::Action::Increase, groupID);
        } else {
            processor->commandQueue.invokeInstantly(Config::Command::ParamB, Config::Command::Action::Decrease, groupID);
        }
//        ControlGroup group = nullptr;
//        if (group == ControlGroup::Group::A) group = processor->controlGroupA;
//        if (group == ControlGroup::Group::B) group = processor->controlGroupB;
//        
//        group.doForEffects([this, increase] (Track* track, Effect* effect) {
//            float value = Parameters::getTrackEffectParam(track->getIndex(), effect->index, Config::Parameter::ParamB);
//            if (increase) value += (float) Config::ParamChangePerStep;
//            if (!increase) value -= (float) Config::ParamChangePerStep;
//            Parameters::setTrackEffectParam(track->getIndex(), effect->index, Config::Parameter::ParamB, value);
//        });
    };

    paramBKnob->onPress = [this] () {
//        app->addSelectedTracksToGroup(index);
    };

    paramBKnob->onRelease = [this] () {
//        app->onRecordLengthRelease();
    };

    paramBKnob->onAlternatePress = [this] () {
        DBG("onAlternatePress");
    };

    paramBKnob->onAlternateRelease = [this] () {
        DBG("onAlternateRelease");
    };

    paramBKnob->setColor(groupColor);
    addAndMakeVisible(paramBKnob);
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
