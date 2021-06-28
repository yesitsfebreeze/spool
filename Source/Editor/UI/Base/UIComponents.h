#pragma once
#define _USE_MATH_DEFINES
 
#include <cmath>
#include <JuceHeader.h>
#include "../../../Processor/SpoolProcessor.h"
#include "../../SpoolEditor.h"
#include "../../EditorConfig.h"


// COMPONENT
class UIComponent : public juce::Component {
public:
    SpoolProcessor* processor;
    SpoolEditor* editor;
    
    int paddingTop = 0;
    int paddingRight = 0;
    int paddingBottom = 0;
    int paddingLeft = 0;

    UIComponent() {
        setOpaque(false);
    }
    
    void setReferences(SpoolProcessor* processor, SpoolEditor* editor) {
        this->processor = processor;
        this->editor = editor;
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


// ANIMATED COMPONENT
class AnimatedUIComponent : public juce::AnimatedAppComponent {
public:
    SpoolProcessor* processor;
    SpoolEditor* editor;

    int paddingTop = 0;
    int paddingRight = 0;
    int paddingBottom = 0;
    int paddingLeft = 0;
    
    AnimatedUIComponent() {
        setOpaque(false);
    }

    void setReferences(SpoolProcessor* processor, SpoolEditor* editor) {
        this->processor = processor;
        this->editor = editor;
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

// BUTTON COMPONENT
class UIButtonComponent : public AnimatedUIComponent {
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

    void mouseDown (const juce::MouseEvent& e) override {
        if (onPress != nullptr) onPress();
    };

    void mouseUp (const juce::MouseEvent& e) override {
        if (onRelease != nullptr) onRelease();
    };

    std::function<void()> onPress;
    std::function<void()> onRelease;
};


class UIKnobComponent : public AnimatedUIComponent {
    
public:
    UIKnobComponent() {
        setOpaque(false);
    }
    
    std::function<void()> onInteract;
    std::function<void(bool increase)> onValueChange;
    std::function<void(bool increase)> onValueChangeAlternate;
    std::function<void()> onPress;
    std::function<void()> onPressAlternate;
    std::function<void()> onRelease;
    std::function<void()> onReleaseAlternate;
    
    
    void setactivationThreshold(int threshold) {
        activationThreshold = threshold;
    }
    
    void setColor(juce::Colour color) {
        knobColor = color;
    }
    
    void setSensitivity(int sens) {
        sensitivity = sens;
        rotationPerStep = degreeToRadians(sens);
    }
    
    void setDotSize(float size) {
        dotSize = size;
    }
    
    void setDotDistanceFromCenter(float distance) {
        dotDistanceFromCenter = distance;
    }
    
    void paint(juce::Graphics& g) override {
        paintRing(g);
        paintDot(g);
    }
    
    enum Direction {
        horizontal,
        vertical
    };
    
    void mouseDrag (const juce::MouseEvent& event) override {
        if (onValueChange == nullptr) return;

        float distanceFromCenterX = event.getDistanceFromDragStartX() + event.getMouseDownX() - center.getX();
        float distanceFromCenterY = event.getDistanceFromDragStartY() + event.getMouseDownY() - center.getY();
        int distance = std::sqrt(std::pow(distanceFromCenterX, 2) + std::pow(distanceFromCenterY, 2) * 1.0);

        if (distance > activationThreshold) {
            bool xlty = std::abs(distanceFromCenterX) > std::abs(distanceFromCenterY);
            direction = (xlty) ? Direction::horizontal : Direction::vertical;
            activated = true;
        }
        
        if (!activated) return;
        
        int value = (direction == Direction::horizontal) ? distanceFromCenterX * -1 : distanceFromCenterY;
        value = value / sensitivity;
        
        if (value > lastValue) {
            if (event.mods.isRightButtonDown()) {
                onValueChangeAlternate(false);
            }else {
                onValueChange(false);
            }
            onInteract();
            angle += rotationPerStep;
            repaint();
        } else if (value < lastValue) {
            if (event.mods.isRightButtonDown()) {
                onValueChangeAlternate(true);
            }else {
                onValueChange(true);
            }
            onInteract();
            angle -= rotationPerStep;
            repaint();
        }

        lastValue = value;
    }
    
    void mouseDown(const juce::MouseEvent& event) override {
        if (event.mods.isRightButtonDown()) {
            if (onPressAlternate != nullptr) {
                onPressAlternate();
                onInteract();
            }
            return;
        }

        if (onPress != nullptr) {
            onPress();
            onInteract();
        }
    }
    
    void mouseUp(const juce::MouseEvent& event) override {
        lastValue = 0;
        activated = false;
        if (event.mods.isRightButtonDown()) {
            if (onReleaseAlternate != nullptr) {
                onReleaseAlternate();
                onInteract();
            }
            return;
        }
        
        if (onRelease != nullptr) {
            onRelease();
            onInteract();
        }
    }


    void resized() override {
        bounds = getLocalBounds();
        center = bounds.getCentre();
    };
    
    void update() override {
        
    };
    
private:
    bool activated = false;
    
    int sensitivity = 10;
    float rotationPerStep = degreeToRadians(sensitivity);
    int activationThreshold = 30;
    
    Direction direction = Direction::horizontal;
    float initialDegree = 0.f;
    
    int lastValue = 0;
    int value = 0;
    
    juce::Point<int> center;
    juce::Rectangle<int> bounds;
    float angle = 0;
    float knobSize = 0;
    float dotSize = 0.15;
    float dotDistanceFromCenter = 0.8;
    
    juce::Colour knobColor = EditorConfig::Colors::Light;

    float degreeToRadians(float degree) {
        return (degree * (M_PI / 180));
    }

    float radiansToDegree(float radians) {
        return radians * 180 / M_PI;
    }
    
    void paintRing(juce::Graphics& g) {
        int width = bounds.getWidth();
        int height = bounds.getHeight();
                
        knobSize = width;
        float factor = bounds.toFloat().getWidth() / bounds.toFloat().getHeight();
        float offsetTop = 0;
        float offsetLeft = 0;
        if (factor > 1) {
            knobSize = height;
            offsetLeft = (width - height) / 2;
        } else if (factor < 1) {
            knobSize = width;
            offsetTop = (height - width) / 2;
        }
        
        offsetTop += EditorConfig::BorderWidth / 2;
        offsetLeft += EditorConfig::BorderWidth / 2;
        knobSize -= EditorConfig::BorderWidth;
        
        juce::Rectangle<float> area {offsetLeft, offsetTop, knobSize, knobSize};
        g.setColour(knobColor);
        g.drawEllipse(area, EditorConfig::BorderWidth);
    }
    
    void paintDot(juce::Graphics& g) {
        float calculatedDotSize = knobSize * dotSize;
        if (calculatedDotSize < EditorConfig::BorderWidth * 2) {
            calculatedDotSize = EditorConfig::BorderWidth * 2;
        }
        
        float radius = ((knobSize - calculatedDotSize) / 2) * dotDistanceFromCenter;
        juce::Point<float> pointOffset {radius * std::sin(angle), radius * std::cos(angle)};

        juce::Rectangle<float> dotArea {
            (center.getX() - calculatedDotSize / 2) + pointOffset.getX(),
            (center.getY() - calculatedDotSize / 2) + pointOffset.getY(),
            calculatedDotSize,
            calculatedDotSize
        };
        g.setColour(EditorConfig::Colors::Light);
        g.fillEllipse(dotArea);
    }
};
