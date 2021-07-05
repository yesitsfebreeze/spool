#pragma once

#include <JuceHeader.h>
#include "Processor/SpoolProcessor.h"
#include "GUI/GUI.h"

class UIComponentAnimated : public juce::AnimatedAppComponent {
public:
    SpoolProcessor* processor;
    GUI* gui;

    int paddingTop = 0;
    int paddingRight = 0;
    int paddingBottom = 0;
    int paddingLeft = 0;
    
    UIComponentAnimated() {
        setOpaque(false);
    }

    void setReferences(SpoolProcessor* processor, GUI* gui) {
        this->processor = processor;
        this->gui = gui;
        onSetReferences();
    };
    
    virtual void onSetReferences() {
        
    };
    
    void setPadding(int padding) {
        setPadding(padding, padding, padding, padding);
    }
    
    void setPadding(int top, int right, int bottom, int left) {
        paddingTop = top;
        paddingRight = right;
        paddingBottom = bottom;
        paddingLeft = left;

        juce::Rectangle<int> b = getBounds();
        int x = b.getX();
        int y = b.getY();
        int w = b.getWidth();
        int h = b.getHeight();
        setBounds(x + paddingLeft, y + paddingTop, w - paddingLeft - paddingRight, h  - paddingTop - paddingBottom);
    }
};
