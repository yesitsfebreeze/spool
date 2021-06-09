#include "ControlGroupUI.h"


void ControlGroupUI::initializeKnobs() {
    int sensitivity = 20;
    
    
    juce::Colour groupColor = EditorConfig::Colors::groupColorOne;
    int recordIncrease = 2;
    if (index == 1) {
        recordIncrease = 1;
        groupColor = EditorConfig::Colors::groupColorTwo;
    }
    
    UIKnobComponent* testKnob1 = knobs.add(new UIKnobComponent());
    testKnob1->setSensitivity(sensitivity);
    testKnob1->setRotationPerStep(360 / sensitivity);
    testKnob1->onValueChange = [this] (bool increase) {
//        app->setRecordLength(increase, recordIncrease);
    };
    testKnob1->onPress = [this] () {
//        app->addSelectedTracksToGroup(index);
    };
    testKnob1->onRelease = [this] () {
//        app->onRecordLengthRelease();
    };
    testKnob1->onPressAlternate = [this] () {
        DBG("onPressAlternate");
    };
    testKnob1->onReleaseAlternate = [this] () {
        DBG("onReleaseAlternate");
    };
    testKnob1->setColor(EditorConfig::Colors::green);
    addAndMakeVisible(testKnob1);

    
    UIKnobComponent* testKnob2 = knobs.add(new UIKnobComponent());
    testKnob2->setSensitivity(sensitivity);
    testKnob2->setRotationPerStep(360 / sensitivity);
    testKnob2->onValueChange = [this] (bool increase) {
//        app->setRecordLength(increase, recordIncrease);
    };
    testKnob2->onPress = [this] () {
//        app->addSelectedTracksToGroup(index);
    };
    testKnob2->onRelease = [this] () {
//        app->onRecordLengthRelease();
    };
    testKnob2->setColor(groupColor);
    addAndMakeVisible(testKnob2);
    
    UIKnobComponent* testKnob3 = knobs.add(new UIKnobComponent());
    testKnob3->setSensitivity(sensitivity);
    testKnob3->setRotationPerStep(360 / sensitivity);
    testKnob3->onValueChange = [this] (bool increase) {
//        app->setRecordLength(increase, recordIncrease);
    };
    testKnob3->onPress = [this] () {
//        app->addSelectedTracksToGroup(index);
    };
    testKnob3->onRelease = [this] () {
//        app->onRecordLengthRelease();
    };
    testKnob3->setColor(groupColor);
    addAndMakeVisible(testKnob3);

    UIKnobComponent* testKnob4 = knobs.add(new UIKnobComponent());
    testKnob4->setSensitivity(sensitivity);
    testKnob4->setRotationPerStep(360 / sensitivity);
    testKnob4->onValueChange = [this] (bool increase) {
//        app->setRecordLength(increase, recordIncrease);
    };
    testKnob4->onPress = [this] () {
//        app->addSelectedTracksToGroup(index);
    };
    testKnob4->onRelease = [this] () {
//        app->onRecordLengthRelease();
    };
    testKnob4->setColor(groupColor);
    addAndMakeVisible(testKnob4);
    
    UIKnobComponent* functionKnob = knobs.add(new UIKnobComponent());
    functionKnob->setSensitivity(sensitivity);
    functionKnob->setRotationPerStep(22.5);
    functionKnob->onPress = [this] () {
//        app->displayRecordLength();
    };
    functionKnob->onValueChangeAlternate = [this, recordIncrease] (bool increase) {
        if (increase) processor->changeRecordLength(+1);
        if (!increase) processor->changeRecordLength(-1);
//        app->setRecordLength(increase, recordIncrease);
    };
    functionKnob->onValueChange = [this, recordIncrease] (bool increase) {
        if (increase) processor->changeRecordLength(+2);
        if (!increase) processor->changeRecordLength(-2);
//        app->setRecordLength(increase, recordIncrease);
    };
    functionKnob->onRelease = [this] () {
//        app->onRecordLengthRelease();
    };
    
    functionKnob->setColor(EditorConfig::Colors::red);
    addAndMakeVisible(functionKnob);


    UIKnobComponent* recordLengthKnob = knobs.add(new UIKnobComponent());
    recordLengthKnob->setSensitivity(sensitivity);
    recordLengthKnob->setRotationPerStep(22.5);
    recordLengthKnob->onPress = [this] () {
//        app->displayRecordLength();
    };
    recordLengthKnob->onPressAlternate = [this] () {
        processor->changeRecordLength(+1);
    };
    recordLengthKnob->onValueChange = [this, recordIncrease] (bool increase) {
        if (increase) processor->changeRecordLength(+2);
        if (!increase) processor->changeRecordLength(-2);
//        app->setRecordLength(increase, recordIncrease);
    };
    recordLengthKnob->onRelease = [this] () {
//        app->onRecordLengthRelease();
    };
    
    recordLengthKnob->setColor(EditorConfig::Colors::red);
    addAndMakeVisible(recordLengthKnob);
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
        
        knob->setPadding(EditorConfig::padding / 2);
    }
    
}
