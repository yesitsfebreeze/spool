#include "UI.h"
#include "../../../Config.h"


UI::UI() {
    addAndMakeVisible(topAreaUI);
    addAndMakeVisible(tracksUI);
    addAndMakeVisible(sequencerUI);
    addAndMakeVisible(peakMeterUI);
    addAndMakeVisible(controlButtonsUI);
    addAndMakeVisible(controlGroupOneUI);
    addAndMakeVisible(controlGroupTwoUI);
}

UI::~UI() {
}

void UI::onSetReferences() {
    topAreaUI.setReferences(processor, editor);
    tracksUI.setReferences(processor, editor);
    sequencerUI.setReferences(processor, editor);
    peakMeterUI.setReferences(processor, editor);
    controlButtonsUI.setReferences(processor, editor);
    controlGroupOneUI.setReferences(processor, editor);
    controlGroupTwoUI.setReferences(processor, editor);
}

void UI::paint(juce::Graphics& g) {
    g.fillAll(Config::Colors::dark);
}


void UI::setSquare() {
    juce::Rectangle<int> parentBounds =  getParentComponent()->getLocalBounds();
    int w = parentBounds.getWidth();
    int h = parentBounds.getHeight();
    
    float factor = (0.0f + w) / (0.0f + h);
    
    if (factor == 1) {
        setBounds(0, 0, w, h);
    } else if (factor > 1) {
        setBounds((w - h) / 2, 0, h, h);
    } else if (factor < 1) {
        setBounds(0, (h - w) / 2, w, w);
    }
}

void UI::resized() {
    setSquare();

    juce::Rectangle<int> bounds =  getLocalBounds();
    int w = bounds.getWidth();
    int h = bounds.getHeight();

    int topArea = h * 0.12;
    int rowHeight = (h - topArea) / 5 ;
    int leftOffset = (w - rowHeight * 4) / 2;
    
    topAreaUI.setBounds(leftOffset, 0, w - leftOffset * 2,topArea);
    topAreaUI.setPadding(Config::padding / 2);

    tracksUI.setBounds(leftOffset, topArea, rowHeight * 4, rowHeight * 4);
    tracksUI.setPadding(Config::padding / 2);

    controlButtonsUI.setBounds(leftOffset, topArea + rowHeight * 4, rowHeight * 4, rowHeight);
    controlButtonsUI.setPadding(0, Config::padding / 2, Config::padding / 2, Config::padding / 2);

    controlGroupOneUI.setBounds(0, topArea, leftOffset, h - topArea);
    controlGroupOneUI.setPadding(rowHeight * 0.3, Config::padding, rowHeight * 0.25, Config::padding);

    controlGroupTwoUI.setBounds(w - leftOffset, topArea, leftOffset, h - topArea);
    controlGroupTwoUI.setPadding(rowHeight * 0.3, Config::padding, rowHeight * 0.25, Config::padding);

    sequencerUI.setBounds(0, 0, leftOffset, topArea);
    sequencerUI.setPadding(Config::padding * 2, Config::padding, Config::padding * 2, Config::padding);
    
    peakMeterUI.setBounds(w - leftOffset, 0, leftOffset, topArea);
    peakMeterUI.setPadding(Config::padding * 2, Config::padding, Config::padding * 2, Config::padding);
    
}
