//#ifndef DOXYGEN
// #include "../utility/juce_CreatePluginFilter.h"
//#endif

#include "../GUI/GUI.h"
#include "StandaloneWindow.h"
#include "StandaloneApp.h"


StandaloneWindow::StandaloneWindow(StandaloneApp& app, const juce::String& title,
                        juce::Colour backgroundColour,
                        juce::PropertySet* settingsToUse,
                        bool takeOwnershipOfSettings,
                        const juce::String& preferredDefaultDeviceName,
                        const juce::AudioDeviceManager::AudioDeviceSetup* preferredSetupOptions,
                        const juce::Array<PluginInOuts>& constrainToConfiguration,
                        bool autoOpenMidiDevices
                        )
    : DocumentWindow (title, GUIConfig::Colors::Dark, DocumentWindow::allButtons, true), owner(app)
{
   #if JUCE_IOS || JUCE_ANDROID
    setTitleBarHeight (0);
   #endif

    pluginHolder.reset(new StandalonePluginHolder(settingsToUse, takeOwnershipOfSettings, preferredDefaultDeviceName, preferredSetupOptions, constrainToConfiguration, autoOpenMidiDevices));

   #if JUCE_IOS || JUCE_ANDROID
    setFullScreen(true);
    setContentOwned(new StandaloneMainComponent(*this), false);
   #else
    setUsingNativeTitleBar(true);
    setContentOwned(new StandaloneMainComponent (*this), true);
    setWindowPosition();
    addConstrainer();
    
    if (auto* processor = getAudioProcessor()) {
        if (auto* editor = processor->getActiveEditor()) {
            setResizable(editor->isResizable(), false);
        }
    }
   #endif
}

StandaloneWindow::~StandaloneWindow() {
   #if (! JUCE_IOS) && (! JUCE_ANDROID)
    if (auto* props = pluginHolder->settings.get()) {
        props->setValue("windowX", getX());
        props->setValue("windowY", getY());
        props->setValue("windowWidth", getWidth());
        props->setValue("windowHeight", getHeight());
    }
   #endif

    pluginHolder->stopPlaying();
    clearContentComponent();
    pluginHolder = nullptr;
    sizeConstrainer = nullptr;
}

void StandaloneWindow::setWindowPosition() {
    auto* props = pluginHolder->settings.get();
    
    if (props == nullptr) {
        centreWithSize(getWidth(), getHeight());
        return;
    }
    
    const int fallbackValue = -999;

    int x = props->getIntValue("windowX", fallbackValue);
    int y = props->getIntValue("windowY", fallbackValue);
    int w = props->getIntValue("windowWidth", fallbackValue);
    int h = props->getIntValue("windowHeight", fallbackValue);

    if (w == fallbackValue) w = GUIConfig::DefaultWindowSize;
    if (h == fallbackValue) h = GUIConfig::DefaultWindowSize;
    
    if (x == fallbackValue && y == fallbackValue) {
        centreWithSize(w, h);
        return;
    }
    
    setBoundsConstrained({ x, y, w, h });
}

void StandaloneWindow::addConstrainer() {
    sizeConstrainer.reset(new juce::ComponentBoundsConstrainer);
    sizeConstrainer->setMinimumSize(GUIConfig::MinimumWindowSize, GUIConfig::MinimumWindowSize);
    sizeConstrainer->setFixedAspectRatio(GUIConfig::WindowAspectRatio);
    setConstrainer(sizeConstrainer.get());
}



void StandaloneWindow::resetToDefaultState() {
    pluginHolder->stopPlaying();
    clearContentComponent();
    pluginHolder->deletePlugin();

    if (auto* props = pluginHolder->settings.get())
        props->removeValue ("filterState");

    pluginHolder->createPlugin();
    setContentOwned (new StandaloneMainComponent (*this), true);
    pluginHolder->startPlaying();
}

void StandaloneWindow::closeButtonPressed()  {
    pluginHolder->savePluginState();
    juce::JUCEApplicationBase::quit();
}

void StandaloneWindow::resized() {
    DocumentWindow::resized();
}

    
