#pragma once

#include <JuceHeader.h>
#include "GUI/GUIConfig.h"

class CommandDefinitions {
public:

    enum CustomCommandFlags {
        isLatching = 1 << 6,
    };
    
    void getAllCommands (juce::Array<juce::CommandID>& commands)
    {
        juce::Array<juce::CommandID> ids {
            Config::Command::ID::Settings,
            Config::Command::ID::Function,
            Config::Command::ID::Mute,
            Config::Command::ID::Play,
            Config::Command::ID::Record,
            Config::Command::ID::Track1,
            Config::Command::ID::Track2,
            Config::Command::ID::Track3,
            Config::Command::ID::Track4,
            Config::Command::ID::Track5,
            Config::Command::ID::Track6,
            Config::Command::ID::Track7,
            Config::Command::ID::Track8,
            Config::Command::ID::Track9,
            Config::Command::ID::Track10,
            Config::Command::ID::Track11,
            Config::Command::ID::Track12,
            Config::Command::ID::Track13,
            Config::Command::ID::Track14,
            Config::Command::ID::Track15,
            Config::Command::ID::Track16
        };

        commands.addArray (ids);
    }
    
    void getCommandInfo (juce::CommandID commandID, juce::ApplicationCommandInfo& result)
    {
        int wantsUpDown = juce::ApplicationCommandInfo::CommandFlags::wantsKeyUpDownCallbacks;
        int hidden = juce::ApplicationCommandInfo::CommandFlags::hiddenFromKeyEditor;

        switch (commandID)
        {
            case Config::Command::ID::Settings:
                result.setInfo("Settings", "Opens settings window", "App", hidden);
                
               #if JUCE_MAC
                result.addDefaultKeypress(GUIConfig::DefaultKeyBind::Settings, juce::ModifierKeys::commandModifier);
               #else
                result.addDefaultKeypress(GUIConfig::DefaultKeyBind::Settings, juce::ModifierKeys::ctrlModifier);
               #endif

                break;
            case Config::Command::ID::Function:
                result.setInfo("Function", "The function key", "Controls", wantsUpDown);
                result.addDefaultKeypress(GUIConfig::DefaultKeyBind::Function, 0);
                break;
            case Config::Command::ID::Mute:
                result.setInfo("Mute", "The mute/cue key", "Controls", wantsUpDown);
                result.addDefaultKeypress(GUIConfig::DefaultKeyBind::Mute, 0);
                break;
            case Config::Command::ID::Play:
                result.setInfo("Play", "The play/pause/stop key", "Controls", wantsUpDown);
                result.addDefaultKeypress(GUIConfig::DefaultKeyBind::Play, 0);
                break;
            case Config::Command::ID::Record:
                result.setInfo("Record", "The record key", "Controls", wantsUpDown);
                result.addDefaultKeypress(GUIConfig::DefaultKeyBind::Record, 0);
                break;
            case Config::Command::ID::Track1:
                result.setInfo("Track/FX 1", "selects track or effect 1", "Tracks", wantsUpDown);
                result.addDefaultKeypress(GUIConfig::DefaultKeyBind::Track1, 0);
                break;
            case Config::Command::ID::Track2:
                result.setInfo("Track/FX 2", "selects track or effect 2", "Tracks", wantsUpDown);
                result.addDefaultKeypress(GUIConfig::DefaultKeyBind::Track2, 0);
                break;
            case Config::Command::ID::Track3:
                result.setInfo("Track/FX 3", "selects track or effect 3", "Tracks", wantsUpDown);
                result.addDefaultKeypress(GUIConfig::DefaultKeyBind::Track3, 0);
                break;
            case Config::Command::ID::Track4:
                result.setInfo("Track/FX 4", "selects track or effect 4", "Tracks", wantsUpDown);
                result.addDefaultKeypress(GUIConfig::DefaultKeyBind::Track4, 0);
                break;
            case Config::Command::ID::Track5:
                result.setInfo("Track/FX 5", "selects track or effect 5", "Tracks", wantsUpDown);
                result.addDefaultKeypress(GUIConfig::DefaultKeyBind::Track5, 0);
                break;
            case Config::Command::ID::Track6:
                result.setInfo("Track/FX 6", "selects track or effect 6", "Tracks", wantsUpDown);
                result.addDefaultKeypress(GUIConfig::DefaultKeyBind::Track6, 0);
                break;
            case Config::Command::ID::Track7:
                result.setInfo("Track/FX 7", "selects track or effect 7", "Tracks", wantsUpDown);
                result.addDefaultKeypress(GUIConfig::DefaultKeyBind::Track7, 0);
                break;
            case Config::Command::ID::Track8:
                result.setInfo("Track/FX 8", "selects track or effect 8", "Tracks", wantsUpDown);
                result.addDefaultKeypress(GUIConfig::DefaultKeyBind::Track8, 0);
                break;
            case Config::Command::ID::Track9:
                result.setInfo("Track/FX 9", "selects track or effect 9", "Tracks", wantsUpDown);
                result.addDefaultKeypress(GUIConfig::DefaultKeyBind::Track9, 0);
                break;
            case Config::Command::ID::Track10:
                result.setInfo("Track/FX 10", "selects track or effect 10", "Tracks", wantsUpDown);
                result.addDefaultKeypress(GUIConfig::DefaultKeyBind::Track10, 0);
                break;
            case Config::Command::ID::Track11:
                result.setInfo("Track/FX 11", "selects track or effect 11", "Tracks", wantsUpDown);
                result.addDefaultKeypress(GUIConfig::DefaultKeyBind::Track11, 0);
                break;
            case Config::Command::ID::Track12:
                result.setInfo("Track/FX 12", "selects track or effect 12", "Tracks", wantsUpDown);
                result.addDefaultKeypress(GUIConfig::DefaultKeyBind::Track12, 0);
                break;
            case Config::Command::ID::Track13:
                result.setInfo("Track/FX 13", "selects track or effect 13", "Tracks", wantsUpDown);
                result.addDefaultKeypress(GUIConfig::DefaultKeyBind::Track13, 0);
                result.addDefaultKeypress(GUIConfig::DefaultKeyBind::Track13Alt, 0);
                break;
            case Config::Command::ID::Track14:
                result.setInfo("Track/FX 14", "selects track or effect 14", "Tracks", wantsUpDown);
                result.addDefaultKeypress(GUIConfig::DefaultKeyBind::Track14, 0);
                break;
            case Config::Command::ID::Track15:
                result.setInfo("Track/FX 15", "selects track or effect 15", "Tracks", wantsUpDown);
                result.addDefaultKeypress(GUIConfig::DefaultKeyBind::Track15, 0);
                break;
            case Config::Command::ID::Track16:
                result.setInfo("Track/FX 16", "selects track or effect 16", "Tracks", wantsUpDown);
                result.addDefaultKeypress(GUIConfig::DefaultKeyBind::Track16, 0);
                break;
            default:
                break;
        }
    }
};
