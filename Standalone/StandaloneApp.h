#pragma once

#include "StandaloneWindow.h"

//==============================================================================
class StandaloneApp : public juce::JUCEApplication {
public:
    StandaloneApp();

    const juce::String getApplicationName() override {
        return JucePlugin_Name;
    }

    const juce::String getApplicationVersion() override {
        return JucePlugin_VersionString;
    }

    bool moreThanOneInstanceAllowed() override {
        return false;
    }

    void anotherInstanceStarted (const juce::String&) override {
        
    }

    virtual StandaloneWindow* createWindow();
    void initialise (const juce::String&) override;
    void shutdown() override;
    void systemRequestedQuit() override;

protected:
    juce::ApplicationProperties appProperties;
    std::unique_ptr<StandaloneWindow> mainWindow;
};


#if JucePlugin_Build_Standalone && JUCE_IOS

using namespace juce;

bool JUCE_CALLTYPE juce_isInterAppAudioConnected()
{
    if (auto holder = StandalonePluginHolder::getInstance())
        return holder->isInterAppAudioConnected();

    return false;
}

void JUCE_CALLTYPE juce_switchToHostApplication()
{
    if (auto holder = StandalonePluginHolder::getInstance())
        holder->switchToHostApplication();
}

Image JUCE_CALLTYPE juce_getIAAHostIcon (int size)
{
    if (auto holder = StandalonePluginHolder::getInstance())
        return holder->getIAAHostIcon (size);

    return Image();
}
#endif

