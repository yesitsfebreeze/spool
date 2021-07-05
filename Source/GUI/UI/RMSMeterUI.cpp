#include "RMSMeterUI.h"

#include "Config.h"

RMSMeterUI::RMSMeterUI() {
    setFramesPerSecond(GUIConfig::FPS);
}

RMSMeterUI::~RMSMeterUI() {
    
}

void RMSMeterUI::paint(juce::Graphics& g) {
    g.fillAll(GUIConfig::Colors::Dark);
    juce::Rectangle<float> bounds = getLocalBounds().toFloat();
    
    int w = bounds.getWidth();
    int h = bounds.getHeight();
    int dotSize = w / 2 * GUIConfig::DotSize;
    int offsetX = (w - dotSize) / 2;
    int spacing = (h - (dotSize * 4)) / 3;
    
    
    
    for (auto peak = 0; peak < 4; peak++) {
        if (peak == 0) g.setColour(GUIConfig::Colors::PeakFour);
        if (peak == 1) g.setColour(GUIConfig::Colors::PeakThree);
        if (peak == 2) g.setColour(GUIConfig::Colors::PeakTwo);
        if (peak == 3) g.setColour(GUIConfig::Colors::PeakOne);


        if (peak == 0) {
            g.fillEllipse(offsetX, 0, dotSize , dotSize);
        } else {
            
            g.fillEllipse(offsetX, (spacing + dotSize) * peak, dotSize , dotSize);
        }
        
    }
    
}

void RMSMeterUI::update() {

}

void RMSMeterUI::resized() {
}
