#pragma once

#include <JuceHeader.h>
#include "Config.h"


class StandaloneMenu {
public:
    juce::StringArray getMenuNames() {
        return { "Functions", "Tracks", "Help" };
    }
    
    juce::PopupMenu getMenuForIndex(juce::ApplicationCommandManager& commandManager, juce::PopupMenu menu, int index) {
        if (index == 0) {
            menu.addCommandItem(&commandManager, Config::Command::ID::Function);
            menu.addCommandItem(&commandManager, Config::Command::ID::Mute);
            menu.addCommandItem(&commandManager, Config::Command::ID::Play);
            menu.addCommandItem(&commandManager, Config::Command::ID::Record);
            return menu;
        }
        
        if (index == 1) {
            menu.addCommandItem(&commandManager, Config::Command::ID::Track1);
            menu.addCommandItem(&commandManager, Config::Command::ID::Track2);
            menu.addCommandItem(&commandManager, Config::Command::ID::Track3);
            menu.addCommandItem(&commandManager, Config::Command::ID::Track4);
            menu.addCommandItem(&commandManager, Config::Command::ID::Track5);
            menu.addCommandItem(&commandManager, Config::Command::ID::Track6);
            menu.addCommandItem(&commandManager, Config::Command::ID::Track7);
            menu.addCommandItem(&commandManager, Config::Command::ID::Track8);
            menu.addCommandItem(&commandManager, Config::Command::ID::Track9);
            menu.addCommandItem(&commandManager, Config::Command::ID::Track10);
            menu.addCommandItem(&commandManager, Config::Command::ID::Track11);
            menu.addCommandItem(&commandManager, Config::Command::ID::Track12);
            menu.addCommandItem(&commandManager, Config::Command::ID::Track13);
            menu.addCommandItem(&commandManager, Config::Command::ID::Track14);
            menu.addCommandItem(&commandManager, Config::Command::ID::Track15);
            menu.addCommandItem(&commandManager, Config::Command::ID::Track16);
            return menu;
        }
        
        if (index == 2) {
            menu.addCommandItem(&commandManager, Config::Command::ID::Settings);
            return menu;
        }

        return menu;
    }
};
