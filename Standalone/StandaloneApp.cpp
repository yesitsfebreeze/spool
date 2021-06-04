#include "StandaloneApp.h"


StandaloneApp::StandaloneApp()
{
    juce::PluginHostType::jucePlugInClientCurrentWrapperType = juce::AudioProcessor::wrapperType_Standalone;

    juce::PropertiesFile::Options options;

    options.applicationName     = getApplicationName();
    options.filenameSuffix      = ".settings";
    options.osxLibrarySubFolder = "Application Support";
   #if JUCE_LINUX
    options.folderName          = "~/.config";
   #else
    options.folderName          = "";
   #endif

    appProperties.setStorageParameters (options);
}

StandaloneWindow* StandaloneApp::createWindow() {
   #ifdef JucePlugin_PreferredChannelConfigurations
    StandalonePluginHolder::PluginInOuts channels[] = { JucePlugin_PreferredChannelConfigurations };
   #endif

    return new StandaloneWindow (
        *this,
        getApplicationName(),
        juce::LookAndFeel::getDefaultLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId),
        appProperties.getUserSettings(),
        false,
        {},
        nullptr
        #ifdef JucePlugin_PreferredChannelConfigurations
            ,juce::Array<StandalonePluginHolder::PluginInOuts>(channels, juce::numElementsInArray (channels))
        #else
            ,{}
        #endif
        #if JUCE_DONT_AUTO_OPEN_MIDI_DEVICES_ON_MOBILE
            ,false
        #endif
    );
}

//==============================================================================
void StandaloneApp::initialise (const juce::String&) {
    mainWindow.reset(createWindow());

    #if JUCE_STANDALONE_FILTER_WINDOW_USE_KIOSK_MODE
        juce::Desktop::getInstance().setKioskModeComponent(mainWindow.get(), false);
    #endif

    mainWindow->setVisible(true);
}

void StandaloneApp::shutdown() {
    mainWindow = nullptr;
    appProperties.saveIfNeeded();
}

//==============================================================================
void StandaloneApp::systemRequestedQuit() {
    if (mainWindow.get() != nullptr) mainWindow->pluginHolder->savePluginState();

    if (juce::ModalComponentManager::getInstance()->cancelAllModalComponents()) {
        juce::Timer::callAfterDelay (100, [] () {
            if (auto app = juce::JUCEApplicationBase::getInstance()) app->systemRequestedQuit();
        });
    } else {
        quit();
    }
}

START_JUCE_APPLICATION(StandaloneApp);
