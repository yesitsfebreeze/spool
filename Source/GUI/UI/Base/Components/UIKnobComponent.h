#pragma once
#define _USE_MATH_DEFINES
#include <math.h>

class UIKnobComponent : public UIComponentAnimated {
    
public:
    UIKnobComponent() {
        setOpaque(false);
    }
    
    std::function<void()> onInteract;
    std::function<void(bool increase)> onValueChange;
    std::function<void(bool increase)> onAlternateValueChange;
    std::function<void()> onPress;
    std::function<void()> onAlternatePress;
    std::function<void()> onRelease;
    std::function<void()> onAlternateRelease;
    
    
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
                if (onAlternateValueChange != nullptr) onAlternateValueChange(false);
            } else {
                if (onValueChange != nullptr) onValueChange(false);
            }
            if (onInteract != nullptr) onInteract();
            angle += rotationPerStep;
            repaint();
        } else if (value < lastValue) {
            if (event.mods.isRightButtonDown()) {
                if (onAlternateValueChange != nullptr) onAlternateValueChange(true);
            } else {
                if (onValueChange != nullptr) onValueChange(true);
            }
            if (onInteract != nullptr) onInteract();
            angle -= rotationPerStep;
            repaint();
        }

        lastValue = value;
    }
    
    void mouseDown(const juce::MouseEvent& event) override {
        if (event.mods.isRightButtonDown()) {
            if (onAlternatePress != nullptr) onAlternatePress();
            if (onInteract != nullptr) onInteract();
            return;
        }

        if (onPress != nullptr) onPress();
        if (onInteract != nullptr) onInteract();
    }
    
    void mouseUp(const juce::MouseEvent& event) override {
        lastValue = 0;
        activated = false;
        if (event.mods.isRightButtonDown()) {
            if (onAlternateRelease != nullptr) onAlternateRelease();
            if (onInteract != nullptr) onInteract();
            return;
        }
        
        if (onRelease != nullptr) onRelease();
        if (onInteract != nullptr) onInteract();
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
    
    juce::Colour knobColor = GUIConfig::Colors::Light;

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
        
        offsetTop += GUIConfig::BorderWidth / 2;
        offsetLeft += GUIConfig::BorderWidth / 2;
        knobSize -= GUIConfig::BorderWidth;
        
        juce::Rectangle<float> area {offsetLeft, offsetTop, knobSize, knobSize};
        g.setColour(knobColor);
        g.drawEllipse(area, GUIConfig::BorderWidth);
    }
    
    void paintDot(juce::Graphics& g) {
        float calculatedDotSize = knobSize * dotSize;
        if (calculatedDotSize < GUIConfig::BorderWidth * 2) {
            calculatedDotSize = GUIConfig::BorderWidth * 2;
        }
        
        float radius = ((knobSize - calculatedDotSize) / 2) * dotDistanceFromCenter;
        juce::Point<float> pointOffset {radius * std::sin(angle), radius * std::cos(angle)};

        juce::Rectangle<float> dotArea {
            (center.getX() - calculatedDotSize / 2) + pointOffset.getX(),
            (center.getY() - calculatedDotSize / 2) + pointOffset.getY(),
            calculatedDotSize,
            calculatedDotSize
        };
        g.setColour(GUIConfig::Colors::Light);
        g.fillEllipse(dotArea);
    }
};
