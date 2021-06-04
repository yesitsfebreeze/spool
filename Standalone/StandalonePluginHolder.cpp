#include "StandalonePluginHolder.h"
#include "StandaloneWindow.h"
#include "StandaloneSettingsComponent.h"


StandalonePluginHolder::StandalonePluginHolder (
                            juce::PropertySet* settingsToUse,
                            bool takeOwnershipOfSettings,
                            const juce::String& preferredDefaultDeviceName,
                            const juce::AudioDeviceManager::AudioDeviceSetup* preferredSetupOptions,
                            const juce::Array<PluginInOuts>& channels, bool shouldAutoOpenMidiDevices
                        ) :
                            settings (settingsToUse, takeOwnershipOfSettings),
                            channelConfiguration (channels),
                            autoOpenMidiDevices (shouldAutoOpenMidiDevices)
{
    shouldMuteInput.addListener (this);
    shouldMuteInput = ! isInterAppAudioConnected();

    createPlugin();

    auto inChannels = (channelConfiguration.size() > 0 ? channelConfiguration[0].numIns : processor->getMainBusNumInputChannels());

    if (preferredSetupOptions != nullptr) {
        options.reset (new juce::AudioDeviceManager::AudioDeviceSetup (*preferredSetupOptions));
    }

    auto audioInputRequired = (inChannels > 0);

    if (audioInputRequired && juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)&& ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio)) {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio, [this, preferredDefaultDeviceName] (bool granted) { init (granted, preferredDefaultDeviceName); });
    } else {
        init (audioInputRequired, preferredDefaultDeviceName);
    }
}

StandalonePluginHolder::~StandalonePluginHolder() {
    stopTimer();
    deletePlugin();
    shutDownAudioDevices();
}

void StandalonePluginHolder::init(bool enableAudioInput, const juce::String& preferredDefaultDeviceName) {
    setupAudioDevices (enableAudioInput, preferredDefaultDeviceName, options.get());
    reloadPluginState();
    startPlaying();

   if (autoOpenMidiDevices) startTimer (500);
}

void StandalonePluginHolder::createPlugin() {
    processor.reset (createPluginFilterOfType (juce::AudioProcessor::wrapperType_Standalone));
    processor->disableNonMainBuses();
    processor->setRateAndBufferSizeDetails (44100, 512);
    int inChannels = (channelConfiguration.size() > 0 ? channelConfiguration[0].numIns : processor->getMainBusNumInputChannels());
    int outChannels = (channelConfiguration.size() > 0 ? channelConfiguration[0].numOuts : processor->getMainBusNumOutputChannels());
    processorHasPotentialFeedbackLoop = (inChannels > 0 && outChannels > 0);
}

void StandalonePluginHolder::deletePlugin() {
    stopPlaying();
    processor = nullptr;
}

void StandalonePluginHolder::startPlaying() {
    player.setProcessor (processor.get());

    #if JucePlugin_Enable_IAA && JUCE_IOS
    if (auto device = dynamic_cast<iOSAudioIODevice*> (deviceManager.getCurrentAudioDevice())) {
        processor->setPlayHead (device->getAudioPlayHead());
        device->setMidiMessageCollector (&player.getMidiMessageCollector());
    }
    #endif
}

void StandalonePluginHolder::stopPlaying() {
    player.setProcessor (nullptr);
}



StandalonePluginHolder* StandalonePluginHolder::getInstance() {
   #if JucePlugin_Enable_IAA || JucePlugin_Build_Standalone
    if (juce::PluginHostType::getPluginLoadedAs() == juce::AudioProcessor::wrapperType_Standalone)
    {
        auto& desktop = juce::Desktop::getInstance();
        const int numTopLevelWindows = desktop.getNumComponents();

        for (int i = 0; i < numTopLevelWindows; ++i)
            if (auto window = dynamic_cast<StandaloneWindow*> (desktop.getComponent (i)))
                return window->getPluginHolder();
    }
   #endif

    return nullptr;
}


void StandalonePluginHolder::audioDeviceIOCallback (const float** inputChannelData, int numInputChannels, float** outputChannelData, int numOutputChannels, int numSamples) {
    if (muteInput) {
        emptyBuffer.clear();
        inputChannelData = emptyBuffer.getArrayOfReadPointers();
    }

    player.audioDeviceIOCallback (inputChannelData, numInputChannels, outputChannelData, numOutputChannels, numSamples);
}

void StandalonePluginHolder::audioDeviceAboutToStart (juce::AudioIODevice* device) {
    emptyBuffer.setSize (device->getActiveInputChannels().countNumberOfSetBits(), device->getCurrentBufferSizeSamples());
    emptyBuffer.clear();

    player.audioDeviceAboutToStart (device);
    player.setMidiOutput (deviceManager.getDefaultMidiOutput());
}

void StandalonePluginHolder::audioDeviceStopped() {
    player.setMidiOutput (nullptr);
    player.audioDeviceStopped();
    emptyBuffer.setSize (0, 0);
}

void StandalonePluginHolder::setupAudioDevices (bool enableAudioInput, const juce::String& preferredDefaultDeviceName, const juce::AudioDeviceManager::AudioDeviceSetup* preferredSetupOptions) {
    deviceManager.addAudioCallback (this);
    deviceManager.addMidiInputDeviceCallback ({}, &player);
    reloadAudioDeviceState (enableAudioInput, preferredDefaultDeviceName, preferredSetupOptions);
}


void StandalonePluginHolder::shutDownAudioDevices() {
    saveAudioDeviceState();

    deviceManager.removeMidiInputDeviceCallback ({}, &player);
    deviceManager.removeAudioCallback (this);
}


void StandalonePluginHolder::timerCallback() {
    auto newMidiDevices = juce::MidiInput::getAvailableDevices();

    if (newMidiDevices != lastMidiDevices) {
        for (auto& oldDevice : lastMidiDevices) {
            if (! newMidiDevices.contains (oldDevice)) deviceManager.setMidiInputDeviceEnabled (oldDevice.identifier, false);
        }

        for (auto& newDevice : newMidiDevices) {
            if (! lastMidiDevices.contains (newDevice)) deviceManager.setMidiInputDeviceEnabled (newDevice.identifier, true);
        }

        lastMidiDevices = newMidiDevices;
    }
}


// -----------------------------------------------------------------------
// SETTINGS
// -----------------------------------------------------------------------

void StandalonePluginHolder::showAudioSettingsDialog()
{
    juce::DialogWindow::LaunchOptions o;

    int maxNumInputs = 0, maxNumOutputs = 0;

    if (channelConfiguration.size() > 0) {
        auto& defaultConfig = channelConfiguration.getReference (0);

        maxNumInputs  = juce::jmax (0, (int) defaultConfig.numIns);
        maxNumOutputs = juce::jmax (0, (int) defaultConfig.numOuts);
    }

    if (auto* bus = processor->getBus (true, 0)) {
        maxNumInputs = juce::jmax (0, bus->getDefaultLayout().size());
    }

    if (auto* bus = processor->getBus (false, 0)) {
        maxNumOutputs = juce::jmax (0, bus->getDefaultLayout().size());
    }

    o.content.setOwned (new SettingsComponent (*this, deviceManager, maxNumInputs, maxNumOutputs));
    o.content->setSize (500, 550);

    o.dialogTitle                   = TRANS("Audio/MIDI Settings");
    o.dialogBackgroundColour        = o.content->getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId);
    o.escapeKeyTriggersCloseButton  = true;
    o.useNativeTitleBar             = true;
    o.resizable                     = false;

    o.launchAsync();
}


void StandalonePluginHolder::saveAudioDeviceState()
{
    if (settings != nullptr)
    {
        auto xml = deviceManager.createStateXml();
        settings->setValue ("audioSetup", xml.get());

        #if ! (JUCE_IOS || JUCE_ANDROID)
        settings->setValue ("shouldMuteInput", (bool) shouldMuteInput.getValue());
        #endif
    }
}

void StandalonePluginHolder::reloadAudioDeviceState (bool enableAudioInput, const juce::String& preferredDefaultDeviceName, const juce::AudioDeviceManager::AudioDeviceSetup* preferredSetupOptions) {
    std::unique_ptr<juce::XmlElement> savedState;

    if (settings != nullptr) {
        savedState = settings->getXmlValue ("audioSetup");

        #if ! (JUCE_IOS || JUCE_ANDROID)
        shouldMuteInput.setValue (settings->getBoolValue ("shouldMuteInput", true));
        #endif
    }

    auto totalInChannels  = processor->getMainBusNumInputChannels();
    auto totalOutChannels = processor->getMainBusNumOutputChannels();

    if (channelConfiguration.size() > 0) {
        auto defaultConfig = channelConfiguration.getReference (0);
        totalInChannels  = defaultConfig.numIns;
        totalOutChannels = defaultConfig.numOuts;
    }

    deviceManager.initialise (enableAudioInput ? totalInChannels : 0, totalOutChannels, savedState.get(), true, preferredDefaultDeviceName,preferredSetupOptions);
}
