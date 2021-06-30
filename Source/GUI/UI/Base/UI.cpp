#include "UI.h"
#include "../../GUIConfig.h"


UI::UI() {
    addAndMakeVisible(topAreaUI);
    addAndMakeVisible(tracksUI);
    addAndMakeVisible(sequencerUI);
    addAndMakeVisible(RMSMeterUI);
    addAndMakeVisible(controlButtonsUI);
    addAndMakeVisible(controlGroupOneUI);
    addAndMakeVisible(controlGroupTwoUI);
}

UI::~UI() {
}

void UI::onSetReferences() {
    topAreaUI.setReferences(processor, gui);
    tracksUI.setReferences(processor, gui);
    sequencerUI.setReferences(processor, gui);
    RMSMeterUI.setReferences(processor, gui);
    controlButtonsUI.setReferences(processor, gui);
    controlGroupOneUI.setReferences(processor, gui);
    controlGroupTwoUI.setReferences(processor, gui);
}

void UI::paint(juce::Graphics& g) {
    g.fillAll(GUIConfig::Colors::Dark);
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

    tracksUI.setBounds(leftOffset, 0, rowHeight * 4, rowHeight * 4);
    tracksUI.setPadding(GUIConfig::Padding / 2);

    controlGroupOneUI.setBounds(0, 0, leftOffset, rowHeight * 3);
    controlGroupOneUI.setPadding(GUIConfig::Padding / 2);
    
    controlGroupTwoUI.setBounds(w - leftOffset, 0, leftOffset, rowHeight * 3);
    controlGroupTwoUI.setPadding(GUIConfig::Padding / 2);

    sequencerUI.setBounds(0, rowHeight * 3, leftOffset, rowHeight);
    sequencerUI.setPadding(GUIConfig::Padding * 2);

    RMSMeterUI.setBounds(w - leftOffset, rowHeight * 3, leftOffset, rowHeight);
    RMSMeterUI.setPadding(GUIConfig::Padding * 1.5, GUIConfig::Padding * 2, GUIConfig::Padding * 1.5, GUIConfig::Padding * 2);

    controlButtonsUI.setBounds(leftOffset, rowHeight * 4, rowHeight * 4, rowHeight);
    controlButtonsUI.setPadding(0, GUIConfig::Padding / 2, GUIConfig::Padding / 2, GUIConfig::Padding / 2);

    
}
