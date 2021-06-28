#pragma once
#include <JuceHeader.h>
#include "StandaloneNotificationComponent.h"
#include "StandaloneMenu.h"

class StandaloneWindow;

class StandaloneMainComponent : public juce::Component, private juce::Value::Listener, private juce::ComponentListener, public juce::MenuBarModel
{
public:
    StandaloneMainComponent (StandaloneWindow& window);
    ~StandaloneMainComponent() override;
    void resized() override;
    
    juce::StringArray getMenuBarNames() override;
    
    juce::PopupMenu getMenuForIndex (int menuIndex, const juce::String& menuName) override;

    void menuItemSelected (int itemID, int index) override;

private:

    
    void componentMovedOrResized (Component&, bool, bool) override;
    juce::Rectangle<int> getSizeToContainEditor() const;

    
    void inputMutedChanged (bool newInputMutedValue);
    void valueChanged (juce::Value& value) override;
    
    StandaloneMenu menu;
    StandaloneWindow& owner;
    std::unique_ptr<juce::AudioProcessorEditor> editor;
    bool shouldShowNotification = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StandaloneMainComponent)
};
