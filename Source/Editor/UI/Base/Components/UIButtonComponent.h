#pragma once

class UIButtonComponent : public UIComponentAnimated {
public:
    juce::Colour fillColor = EditorConfig::Colors::Light;
    juce::Colour originalFillColor = fillColor;
    
    juce::Colour borderColor = EditorConfig::Colors::Light;
    juce::Colour originalBorderColor = fillColor;
    
    int index = 0;
    bool isSwitch = false;
    bool isDepressed = false;
    
    juce::Point<int> position {0,0};
    juce::Point<int> grid {0,0};
    int buttonX = 0;
    int buttonY = 0;
    int buttonWidth = 0;
    int buttonHeight = 0;
    juce::Graphics* graphics;
    
    UIButtonComponent(int index) {
        setFramesPerSecond(EditorConfig::FPS);
        setMouseCursor(juce::MouseCursor::StandardCursorType::PointingHandCursor);
        this->index = index;
    };

    ~UIButtonComponent() {};
    
    void setColors(juce::Colour fill, juce::Colour border) {
        borderColor = border;
        originalBorderColor = border;
        fillColor = fill;
        originalFillColor = fill;
    }
    
    void paint (juce::Graphics& g) override {
        graphics = &g;
        g.fillAll(EditorConfig::Colors::Dark);
        calculateButtonSize();
        getButtonColors();
        beforePaint();
        drawButton();
        afterPaint();
    };
    
    virtual void beforePaint() {
        
    }
    
    virtual void afterPaint() {
        
    }
    
    virtual void getButtonColors() {
        fillColor = EditorConfig::Colors::Dark;
    
        if (isDepressed) {
            fillColor = originalFillColor;
        }
    }
    
    virtual void calculateButtonSize() {
        juce::Rectangle<float> bounds = getLocalBounds().toFloat();

        buttonX = EditorConfig::BorderWidth / 2;
        buttonY = EditorConfig::BorderWidth / 2;
        buttonWidth = bounds.getWidth() - EditorConfig::BorderWidth;
        buttonHeight = bounds.getHeight() - EditorConfig::BorderWidth;
    }
    
    virtual void drawButton() {
        int p = EditorConfig::Padding / 2;
        
        graphics->setColour(fillColor);
        graphics->fillRoundedRectangle(buttonX + p , buttonY + p , buttonWidth - p * 2 , buttonHeight - p * 2, EditorConfig::BorderRadius);
        
        graphics->setColour(borderColor);
        graphics->drawRoundedRectangle(buttonX + p , buttonY + p , buttonWidth - p * 2 , buttonHeight - p * 2, EditorConfig::BorderRadius, EditorConfig::BorderWidth);
    }
    
    void resized() override {
        
    };


    void calculateBounds(juce::Rectangle<int> bounds, int buttonIndex, int columns,  int rows = 1, float heightFactor = 1) {
        grid.setX(columns);
        grid.setY(rows);
        position.setX(buttonIndex % columns);
        position.setY(buttonIndex / columns);

        auto buttonWidth = (bounds.getWidth()) / grid.x;
        auto buttonHeight = (bounds.getHeight()) / grid.y;
        
        int x = buttonWidth * position.x;
        int y = buttonHeight * position.y;
        int w = buttonWidth;
        int h = buttonHeight * heightFactor;
        
        setBounds(x, y, w, h);
    };
    

    void update() override {
        
    };

    void mouseDown (const juce::MouseEvent& event) override {
        if (event.mods.isRightButtonDown()) {
            if (onAlternatePress != nullptr) onAlternatePress();
            return;
        }
        if (onPress != nullptr) onPress();
    };

    void mouseUp (const juce::MouseEvent& event) override {
        if (event.mods.isRightButtonDown()) {
            if (onAlternateRelease != nullptr) onAlternateRelease();
            return;
        }
        if (onRelease != nullptr) onRelease();
    };

    std::function<void()> onPress;
    std::function<void()> onAlternatePress;
    std::function<void()> onRelease;
    std::function<void()> onAlternateRelease;
};
