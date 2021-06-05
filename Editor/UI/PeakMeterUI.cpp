#include "PeakMeterUI.h"
#include "../../Config.h"

PeakMeterUI::PeakMeterUI() {
    setFramesPerSecond(Config::uiFPS);
}

PeakMeterUI::~PeakMeterUI() {
    
}

void PeakMeterUI::paint(juce::Graphics& g) {
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
    
    int borderRadius = barWidth / 2;
    
    if (Config::borderRadius == 0) {
        borderRadius = 0;
    }
    
    for (auto peak = 0; peak < 4; peak++) {
        int barOffsetX = xOffset * peak + peak * barWidth;
        
        if (peak == 0) {
            g.setColour(Config::Colors::peakOne);
        }
        if (peak == 1) {
            g.setColour(Config::Colors::peakTwo);
        }
        if (peak == 2) {
            g.setColour(Config::Colors::peakThree);
        }
        if (peak == 3) {
            g.setColour(Config::Colors::peakFour);
            
        }
        
        
        
        g.fillRoundedRectangle(barOffsetX, yOffset, barWidth, barHeight, borderRadius);
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

void PeakMeterUI::update() {

}

void PeakMeterUI::resized() {
}
