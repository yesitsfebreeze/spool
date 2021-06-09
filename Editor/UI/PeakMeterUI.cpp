#include "PeakMeterUI.h"
#include "../../Config.h"

PeakMeterUI::PeakMeterUI() {
    setFramesPerSecond(EditorConfig::FPS);
}

PeakMeterUI::~PeakMeterUI() {
    
}

void PeakMeterUI::paint(juce::Graphics& g) {
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
    
    int borderRadius = barWidth / 2;
    
    if (EditorConfig::borderRadius == 0) {
        borderRadius = 0;
    }
    
    for (auto peak = 0; peak < 4; peak++) {
        int barOffsetX = xOffset * peak + peak * barWidth;
        
        if (peak == 0) {
            g.setColour(EditorConfig::Colors::peakOne);
        }
        if (peak == 1) {
            g.setColour(EditorConfig::Colors::peakTwo);
        }
        if (peak == 2) {
            g.setColour(EditorConfig::Colors::peakThree);
        }
        if (peak == 3) {
            g.setColour(EditorConfig::Colors::peakFour);
            
        }
        
        
        
        g.fillRoundedRectangle(barOffsetX, yOffset, barWidth, barHeight, borderRadius);
        g.drawRoundedRectangle(
           barOffsetX + EditorConfig::borderWidth / 2,
           yOffset + EditorConfig::borderWidth / 2,
           barWidth - EditorConfig::borderWidth,
           barHeight - EditorConfig::borderWidth,
           borderRadius,
           EditorConfig::borderWidth
       );
    }
}

void PeakMeterUI::update() {

}

void PeakMeterUI::resized() {
}
