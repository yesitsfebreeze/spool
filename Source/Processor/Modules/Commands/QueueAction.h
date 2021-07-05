#pragma once
#include <JuceHeader.h>
#include <map>

#include "Config.h"
#include "Processor/Modules/ControlGroup/ControlGroup.h"
#include "Processor/Modules/Commands/CommandTypes.h"

struct QueueAction {
public:
    QueueAction(
        Config::Command::ID cmdID,
        juce::int64 executionTime,
        CommandTypes::Trigger trigger,
        std::map<const CommandTypes::Trigger, std::function<void(QueueAction* action)>> callbacks,
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
    CommandTypes::Trigger trigger = CommandTypes::Trigger::Instant;
    std::map<const CommandTypes::Trigger, std::function<void(QueueAction* action)>> callbacks;
    bool isFunctionDown = false;
    ControlGroup::Group cmdGroup = ControlGroup::Group::Unassinged;
};
