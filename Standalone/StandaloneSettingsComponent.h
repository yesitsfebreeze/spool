#pragma once

#include <JuceHeader.h>
#include "StandalonePluginHolder.h"

//TODO: open settings window with menu and shortcut

class SettingsComponent : public juce::Component {
public:
    SettingsComponent (StandalonePluginHolder& pluginHolder, juce::AudioDeviceManager& deviceManagerToUse, int maxAudioInputChannels, int maxAudioOutputChannels)
        : owner (pluginHolder),
          deviceSelector (deviceManagerToUse,
                          0, maxAudioInputChannels,
                          0, maxAudioOutputChannels,
                          true,
                          (pluginHolder.processor.get() != nullptr && pluginHolder.processor->producesMidi()),
                          true, false),
          shouldMuteLabel  ("Feedback Loop:", "Feedback Loop:"),
          shouldMuteButton ("Mute audio input")
    {
        setOpaque (true);

        shouldMuteButton.setClickingTogglesState (true);
        shouldMuteButton.getToggleStateValue().referTo (owner.shouldMuteInput);

        addAndMakeVisible (deviceSelector);

        if (owner.getProcessorHasPotentialFeedbackLoop())
        {
            addAndMakeVisible (shouldMuteButton);
            addAndMakeVisible (shouldMuteLabel);

            shouldMuteLabel.attachToComponent (&shouldMuteButton, true);
        }
    }

    void paint (juce::Graphics& g) override
    {
        g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    }

    void resized() override
    {
        auto r = getLocalBounds();

        if (owner.getProcessorHasPotentialFeedbackLoop())
        {
            auto itemHeight = deviceSelector.getItemHeight();
            auto extra = r.removeFromTop (itemHeight);

            auto seperatorHeight = (itemHeight >> 1);
            shouldMuteButton.setBounds (juce::Rectangle<int> (extra.proportionOfWidth (0.35f), seperatorHeight, extra.proportionOfWidth (0.60f), deviceSelector.getItemHeight()));

            r.removeFromTop (seperatorHeight);
        }

        deviceSelector.setBounds (r);
    }

private:
    //==============================================================================
    StandalonePluginHolder& owner;
    juce::AudioDeviceSelectorComponent deviceSelector;
    juce::Label shouldMuteLabel;
    juce::ToggleButton shouldMuteButton;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SettingsComponent)
};
