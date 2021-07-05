#pragma once

#include <JuceHeader.h>
#include "../../../Config.h"
#include "../ControlGroup/ControlGroup.h"
#include "QueueAction.h"
#include "CommandTypes.h"

struct QueueCommand {
public:
    Config::Command::ID cmdID;
    ControlGroup::Group cmdGroup;
    
    std::map<const CommandTypes::Action, std::map<const CommandTypes::Trigger, std::function<void(QueueAction* action)>>> callbacks = {};
    
    bool isPressed = false;
    bool isReleased = true;
    bool wasInstantPressed = false;
    bool wasPressed = false;
    bool wasDoublePressed = false;
    bool wasDoubleHeld = false;
    bool wasHeld = false;
    
    int pressCounter = 0;
    juce::int64 triggerTime = 0;
    juce::int64 holdTriggerTime = 0;
    juce::int64 doublePressTriggerTime = 0;
    
    bool clearedPress = false;
    bool clearedDoublePress = false;

    int debounceTimeout = Config::DebounceTimeout;
    int debounceBeats = 1;
};
