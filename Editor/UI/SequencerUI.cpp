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
    
    int barWidth = w / 6;
    float xOffset = (w - barWidth * 4) / 3;
    int barHeight = h;
    float yOffset = 0;
    
    if (barWidth < 5) barWidth = 5;
    if (barHeight < 5) barHeight = 5;
    

    int tick = sequencerTick % 4;
    int borderRadius = (barWidth - EditorConfig::borderWidth) / 2;
    if (borderRadius > EditorConfig::borderRadius) borderRadius = EditorConfig::borderRadius;
    
    if (EditorConfig::borderRadius == 0) {
        borderRadius = 0;
    }
    
    for (auto beat = 0; beat < 4; beat++) {
        
        int barOffsetX = xOffset * beat + beat * barWidth;
        if (!isRunning) {
            g.setColour(EditorConfig::Colors::light);
        } else {
            if (beat == tick) {
                g.setColour(EditorConfig::Colors::red);
                g.fillRoundedRectangle(barOffsetX, yOffset, barWidth, barHeight, borderRadius);
            } else if (beat < tick) {
                g.setColour(EditorConfig::Colors::darkRed);
            } else {
                g.setColour(EditorConfig::Colors::mid);
            }
        }
        
        g.fillRoundedRectangle(barOffsetX, yOffset, barWidth, barHeight, borderRadius);
    }
}


void SequencerUI::update() {
    sequencerTick = processor->sequencer->tick;
    isRunning = processor->sequencer->isRunning;
}

void SequencerUI::resized() {
}
