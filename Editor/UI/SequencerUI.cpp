#include "SequencerUI.h"
#include "../../Config.h"
#include "../../Processor/Modules/Sequencer.h"

SequencerUI::SequencerUI() {
    setFramesPerSecond(Config::uiFPS);
}

SequencerUI::~SequencerUI() {
    
}


void SequencerUI::paint(juce::Graphics& g) {
    g.fillAll(Config::Colors::dark);
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
    int borderRadius = barWidth / 2;
    if (borderRadius > Config::borderRadius) borderRadius = Config::borderRadius;
    
    if (Config::borderRadius == 0) {
        borderRadius = 0;
    }
    
    for (auto beat = 0; beat < 4; beat++) {
        
        int barOffsetX = xOffset * beat + beat * barWidth;
        if (!isRunning) {
            g.setColour(Config::Colors::light);
        } else {
            if (beat == tick) {
                g.setColour(Config::Colors::red);
                g.fillRoundedRectangle(barOffsetX, yOffset, barWidth, barHeight, borderRadius);
            } else if (beat < tick) {
                g.setColour(Config::Colors::red);
            } else {
                g.setColour(Config::Colors::light);
            }
        }
        
        g.drawRoundedRectangle(
           barOffsetX + Config::borderWidth / 2,
           yOffset + Config::borderWidth / 2,
           barWidth - Config::borderWidth,
           barHeight - Config::borderWidth,
           borderRadius,
           Config::borderWidth
       );
    }
}


void SequencerUI::update() {
    sequencerTick = processor->sequencer->tick;
    isRunning = processor->sequencer->isRunning;
}

void SequencerUI::resized() {
}
