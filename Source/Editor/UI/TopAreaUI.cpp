#include "TopAreaUI.h"
#include "../../Config.h"


TopAreaUI::~TopAreaUI() {
    bypassKnob.reset();
}

void TopAreaUI::initializeKnobs() {
    int sensitivity = 2;

    bypassKnob.reset(new UIKnobComponent());
    bypassKnob->setSensitivity(sensitivity);
    bypassKnob->setRotationPerStep(360 * ((float)sensitivity / 100));
    bypassKnob->onPress = [this] () {


    };
    bypassKnob->onValueChange = [this] (bool increase) {
//        double currentBpm = app->sequencer.getBpm();
        
//        if (increase) {
//            app->sequencer.setBpm(currentBpm + 0.5f);
//        } else {
//            app->sequencer.setBpm(currentBpm - 0.5f);
//        }
    };
    bypassKnob->onRelease = [this] () {

    };

    bypassKnob->setColor(EditorConfig::Colors::GroupColorTwo);
    addAndMakeVisible(bypassKnob.get());
}


void TopAreaUI::resized() {
    juce::Rectangle<int> bounds = getLocalBounds();
    juce::Point<int> center = bounds.getCentre();

    int knobSize = bounds.getHeight() * 0.75;
    bypassKnob->setBounds(center.getX() - knobSize / 2, center.getY() - knobSize / 2, knobSize, knobSize);
}
