#pragma once
#include <JuceHeader.h>

//==============================================================================
class StandaloneNotificationComponent : public juce::Component
{
public:
    enum { height = 30 };

    StandaloneNotificationComponent (juce::Button::Listener* settingsButtonListener)
        : notification ("notification", "Audio input is muted to avoid feedback loop"),
         #if JUCE_IOS || JUCE_ANDROID
          settingsButton ("Unmute Input")
         #else
          settingsButton ("Settings...")
         #endif
    {
        setOpaque (true);

        notification.setColour (juce::Label::textColourId, juce::Colours::black);

        settingsButton.addListener (settingsButtonListener);

        addAndMakeVisible (notification);
        addAndMakeVisible (settingsButton);
    }

    void paint (juce::Graphics& g) override
    {
        auto r = getLocalBounds();

        g.setColour (juce::Colours::darkgoldenrod);
        g.fillRect (r.removeFromBottom (1));

        g.setColour (juce::Colours::lightgoldenrodyellow);
        g.fillRect (r);
    }

    void resized() override
    {
        auto r = getLocalBounds().reduced (5);

        settingsButton.setBounds (r.removeFromRight (70));
        notification.setBounds (r);
    }
private:
    juce::Label notification;
    juce::TextButton settingsButton;
};
