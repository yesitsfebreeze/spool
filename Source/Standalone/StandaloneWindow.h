#pragma once

//#ifndef DOXYGEN
// #include "../utility/juce_CreatePluginFilter.h"
//#endif

#include "Config.h"

#include "Standalone/StandalonePluginHolder.h"
#include "Standalone/StandaloneMainComponent.h"

class StandaloneApp;

class StandaloneWindow : public juce::DocumentWindow {
public:
    //==============================================================================
    typedef StandalonePluginHolder::PluginInOuts PluginInOuts;


    StandaloneWindow (
        StandaloneApp& app,
        const juce::String& title,
        juce::Colour backgroundColour,
        juce::PropertySet* settingsToUse,
        bool takeOwnershipOfSettings,
        const juce::String& preferredDefaultDeviceName = juce::String(),
        const juce::AudioDeviceManager::AudioDeviceSetup* preferredSetupOptions = nullptr,
        const juce::Array<PluginInOuts>& constrainToConfiguration = {},
        #if JUCE_ANDROID || JUCE_IOS
            bool autoOpenMidiDevices = true
        #else
            bool autoOpenMidiDevices = false
        #endif
    );
      

    ~StandaloneWindow() override;
    
    void setWindowPosition();
    void addConstrainer();
    void resetToDefaultState();
    void closeButtonPressed() override;
    void resized() override;
    

    juce::AudioProcessor* getAudioProcessor() const noexcept {
        return pluginHolder->processor.get();
    }

    juce::AudioDeviceManager& getDeviceManager() const noexcept {
        return pluginHolder->deviceManager;
    }

    virtual StandalonePluginHolder* getPluginHolder() {
        return pluginHolder.get();
    }

    
    std::unique_ptr<StandalonePluginHolder> pluginHolder;
    StandaloneApp& owner;
private:
    std::unique_ptr<juce::ComponentBoundsConstrainer> sizeConstrainer;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StandaloneWindow)
};

