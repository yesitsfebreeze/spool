#include "RMSMeterUI.h"
#include "../../Config.h"

RMSMeterUI::RMSMeterUI() {
    setFramesPerSecond(EditorConfig::FPS);
}

RMSMeterUI::~RMSMeterUI() {
    
}

void RMSMeterUI::paint(juce::Graphics& g) {
    g.fillAll(EditorConfig::Colors::Dark);
    juce::Rectangle<float> bounds = getLocalBounds().toFloat();
    
    int w = bounds.getWidth();
    int h = bounds.getHeight();
    int dotSize = w / 2 * EditorConfig::DotSize;
    int offsetX = (w - dotSize) / 2;
    int spacing = (h - (dotSize * 4)) / 3;
    
    
    
    for (auto peak = 0; peak < 4; peak++) {
        if (peak == 0) g.setColour(EditorConfig::Colors::PeakFour);
        if (peak == 1) g.setColour(EditorConfig::Colors::PeakThree);
        if (peak == 2) g.setColour(EditorConfig::Colors::PeakTwo);
        if (peak == 3) g.setColour(EditorConfig::Colors::PeakOne);


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
