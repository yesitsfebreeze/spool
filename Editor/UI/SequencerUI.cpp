#include "SequencerUI.h"
#include "../../Config.h"
#include "../../Processor/Modules/Sequencer/Sequencer.h"

SequencerUI::SequencerUI() {
    setFramesPerSecond(EditorConfig::FPS);
}

SequencerUI::~SequencerUI() {
    
}


void SequencerUI::paint(juce::Graphics& g) {
    g.fillAll(EditorConfig::Colors::dark);
    juce::Rectangle<float> bounds = getLocalBounds().toFloat();
    
    int w = bounds.getWidth();
    int h = bounds.getHeight();
    int dotSize = (w / 2) / 3;
    
    float factor = (float)w/float(h);
    
    int marginHorizontal = (factor > 1) ? (w - h) / 2 : 0;
    int marginVertical = (factor < 1) ? (h - w) / 2 : 0;
    
    
    DBG(marginHorizontal);
    DBG(marginVertical);
    
    int top = marginVertical;
    int right = w - dotSize - marginHorizontal;
    int bottom = h - dotSize - marginVertical;
    int left = marginHorizontal;
    
    int tick = sequencerTick % 4;
    if (tick == 0) {
        if (!lightStateChanged) {
            lightState = !lightState;
            lightStateChanged = true;
        }
    } else {
        lightStateChanged = false;
    };
    
    for (auto dot = 0; dot < 4; dot++) {
        juce::Point<int> pos;
        
        if (dot == 0) pos.setXY(left, top);
        if (dot == 1) pos.setXY(right, top);
        if (dot == 2) pos.setXY(right, bottom);
        if (dot == 3) pos.setXY(left, bottom);
        
        juce::Colour inactiveColor = EditorConfig::Colors::mid;
        juce::Colour activeColor = EditorConfig::Colors::red;

        if (!isRunning) {
            g.setColour(inactiveColor);
        } else {
            if (dot == tick) {
                g.setColour(lightState ? activeColor : inactiveColor);
            } else if (dot < tick) {
                g.setColour(lightState ? activeColor : inactiveColor);
            } else {
                g.setColour(lightState ? inactiveColor : activeColor);
            }
        }
        
        g.fillEllipse(pos.x, pos.y, dotSize, dotSize);
    }
}


void SequencerUI::update() {
    sequencerTick = processor->sequencer->tick;
    isRunning = processor->sequencer->isRunning;
}

void SequencerUI::resized() {
}
