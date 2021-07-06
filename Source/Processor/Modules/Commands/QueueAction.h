#pragma once
#include <JuceHeader.h>
#include <map>

#include "Config.h"
#include "Processor/Modules/ControlGroup/ControlGroup.h"

struct QueueAction {
public:
    QueueAction(
        Config::Command::ID cmdID,
        juce::int64 executionTime,
        Config::Command::Trigger trigger,
        std::map<const Config::Command::Trigger, std::function<void(QueueAction* action)>> callbacks,
        bool isFunctionDown = false,
        ControlGroup::Group cmdGroup = ControlGroup::Group::Unassinged
    ) :
    cmdID(cmdID),
    executionTime(executionTime),
    trigger(trigger),
    callbacks(callbacks),
    isFunctionDown(isFunctionDown),
    cmdGroup(cmdGroup) {}

    Config::Command::ID cmdID;
    juce::int64 executionTime = 0;
    Config::Command::Trigger trigger = Config::Command::Trigger::Instant;
    std::map<const Config::Command::Trigger, std::function<void(QueueAction* action)>> callbacks;
    bool isFunctionDown = false;
    ControlGroup::Group cmdGroup = ControlGroup::Group::Unassinged;
};
