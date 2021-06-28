#pragma once

#include <JuceHeader.h>

class SettingsComponent;

class StandalonePluginHolder : private juce::AudioIODeviceCallback, private juce::Timer, private juce::Value::Listener {
public:

    struct PluginInOuts   { short numIns, numOuts; };

    //==============================================================================
    /** Creates an instance of the default plugin.

        The settings object can be a PropertySet that the class should use to store its
        settings - the takeOwnershipOfSettings indicates whether this object will delete
        the settings automatically when no longer needed. The settings can also be nullptr.

        A default device name can be passed in.

        Preferably a complete setup options object can be used, which takes precedence over
        the preferredDefaultDeviceName and allows you to select the input & output device names,
        sample rate, buffer size etc.

        In all instances, the settingsToUse will take precedence over the "preferred" options if not null.
    */
    StandalonePluginHolder (juce::PropertySet* settingsToUse,
                            bool takeOwnershipOfSettings = true,
                            const juce::String& preferredDefaultDeviceName = juce::String(),
                            const juce::AudioDeviceManager::AudioDeviceSetup* preferredSetupOptions = nullptr,
                            const juce::Array<PluginInOuts>& channels = juce::Array<PluginInOuts>(),
                           #if JUCE_ANDROID || JUCE_IOS
                            bool shouldAutoOpenMidiDevices = true
                           #else
                            bool shouldAutoOpenMidiDevices = false
                           #endif
    );


    ~StandalonePluginHolder() override;

    
    void init(bool enableAudioInput, const juce::String& preferredDefaultDeviceName);
    virtual void createPlugin();
    virtual void deletePlugin();

    

    
    juce::Value& getMuteInputValue() { return shouldMuteInput; }
    bool getProcessorHasPotentialFeedbackLoop() const { return processorHasPotentialFeedbackLoop; }
    void valueChanged (juce::Value& value) override { muteInput = (bool) value.getValue(); }

    //==============================================================================
    
    static juce::String getFilePatterns (const juce::String& fileSuffix)
    {
        if (fileSuffix.isEmpty())
            return {};

        return (fileSuffix.startsWithChar ('.') ? "*" : "*.") + fileSuffix;
    }
    
    juce::File getLastFile() const
    {
        juce::File f;

        if (settings != nullptr)
            f = juce::File (settings->getValue ("lastStateFile"));

        if (f == juce::File())
            f = juce::File::getSpecialLocation (juce::File::userDocumentsDirectory);

        return f;
    }

    void setLastFile (const juce::FileChooser& fc)
    {
        if (settings != nullptr) settings->setValue ("lastStateFile", fc.getResult().getFullPathName());
    }

    /** Pops up a dialog letting the user save the processor's state to a file. */
    void askUserToSaveState (const juce::String& fileSuffix = juce::String())
    {
       #if JUCE_MODAL_LOOPS_PERMITTED
        juce::FileChooser fc (TRANS("Save current state"), getLastFile(), getFilePatterns (fileSuffix));

        if (fc.browseForFileToSave (true))
        {
            setLastFile (fc);

            juce::MemoryBlock data;
            processor->getStateInformation (data);

            if (! fc.getResult().replaceWithData (data.getData(), data.getSize()))
                juce::AlertWindow::showMessageBoxAsync (juce::AlertWindow::WarningIcon,
                                                  TRANS("Error whilst saving"),
                                                  TRANS("Couldn't write to the specified file!"));
        }
       #else
        ignoreUnused (fileSuffix);
       #endif
    }

    /** Pops up a dialog letting the user re-load the processor's state from a file. */
    void askUserToLoadState (const juce::String& fileSuffix = juce::String())
    {
       #if JUCE_MODAL_LOOPS_PERMITTED
        juce::FileChooser fc (TRANS("Load a saved state"), getLastFile(), getFilePatterns (fileSuffix));

        if (fc.browseForFileToOpen())
        {
            setLastFile (fc);

            juce::MemoryBlock data;

            if (fc.getResult().loadFileAsData (data))
                processor->setStateInformation (data.getData(), (int) data.getSize());
            else
                juce::AlertWindow::showMessageBoxAsync (juce::AlertWindow::WarningIcon,
                                                  TRANS("Error whilst loading"),
                                                  TRANS("Couldn't read from the specified file!"));
        }
       #else
        ignoreUnused (fileSuffix);
       #endif
    }

    //==============================================================================
    void startPlaying();
    void stopPlaying();

    //==============================================================================
    /** Shows an audio properties dialog box modally. */
    void showAudioSettingsDialog();
    void saveAudioDeviceState();

    void reloadAudioDeviceState (bool enableAudioInput, const juce::String& preferredDefaultDeviceName, const juce::AudioDeviceManager::AudioDeviceSetup* preferredSetupOptions);

    //==============================================================================
    void savePluginState()
    {
        if (settings != nullptr && processor != nullptr)
        {
            juce::MemoryBlock data;
            processor->getStateInformation (data);

            settings->setValue ("filterState", data.toBase64Encoding());
        }
    }

    void reloadPluginState()
    {
        if (settings != nullptr)
        {
            juce::MemoryBlock data;

            if (data.fromBase64Encoding (settings->getValue ("filterState")) && data.getSize() > 0)
                processor->setStateInformation (data.getData(), (int) data.getSize());
        }
    }

    //==============================================================================
    void switchToHostApplication()
    {
       #if JUCE_IOS
        if (auto device = dynamic_cast<iOSAudioIODevice*> (deviceManager.getCurrentAudioDevice()))
            device->switchApplication();
       #endif
    }

    bool isInterAppAudioConnected()
    {
       #if JUCE_IOS
        if (auto device = dynamic_cast<iOSAudioIODevice*> (deviceManager.getCurrentAudioDevice()))
            return device->isInterAppAudioConnected();
       #endif

        return false;
    }

    juce::Image getIAAHostIcon (int size)
    {
       #if JUCE_IOS && JucePlugin_Enable_IAA
        if (auto device = dynamic_cast<iOSAudioIODevice*> (deviceManager.getCurrentAudioDevice()))
            return device->getIcon (size);
       #else
        juce::ignoreUnused (size);
       #endif

        return {};
    }

    static StandalonePluginHolder* getInstance();

    //==============================================================================
    juce::OptionalScopedPointer<juce::PropertySet> settings;
    std::unique_ptr<juce::AudioProcessor> processor;
    juce::AudioDeviceManager deviceManager;
    juce::AudioProcessorPlayer player;
    juce::Array<PluginInOuts> channelConfiguration;

    // avoid feedback loop by default
    bool processorHasPotentialFeedbackLoop = true;
    std::atomic<bool> muteInput { true };
    juce::Value shouldMuteInput;
    juce::AudioBuffer<float> emptyBuffer;
    bool autoOpenMidiDevices;

    std::unique_ptr<juce::AudioDeviceManager::AudioDeviceSetup> options;
    juce::Array<juce::MidiDeviceInfo> lastMidiDevices;

private:
    void audioDeviceIOCallback (const float** inputChannelData,int numInputChannels, float** outputChannelData, int numOutputChannels, int numSamples) override;
    void audioDeviceAboutToStart (juce::AudioIODevice* device) override;
    void audioDeviceStopped() override;

    void setupAudioDevices (bool enableAudioInput, const juce::String& preferredDefaultDeviceName, const juce::AudioDeviceManager::AudioDeviceSetup* preferredSetupOptions);
    void shutDownAudioDevices();
    
    void timerCallback() override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StandalonePluginHolder)
};
