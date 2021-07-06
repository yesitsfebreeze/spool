#pragma once
#include <map>

#include <JuceHeader.h>
#include "QueueCommand.h"

#include "Config.h"
#include "Processor/Modules/ControlGroup/ControlGroup.h"
#include "Processor/Modules/Commands/QueueAction.h"


class CommandQueue {
public:
    juce::int64 currentTime = 0;
    std::vector<QueueAction*> queue;
    std::map<const int, QueueCommand*> commands = {};
    
    Config::Command::ID FNCommandID = Config::Command::Function;
    bool _isFunctionDown = false;
    bool canExecuteFnFunctions = true;
    
    void registerCommand(Config::Command::ID commandID, Config::Command::Action action, Config::Command::Trigger trigger, std::function<void(QueueAction* action)> callback, ControlGroup::Group group = ControlGroup::Group::Unassinged) {
        QueueCommand* cmd = getExistingCommand(commandID, group);
        
        cmd->callbacks[action][trigger] = callback;
    };
    
    bool invokeInstantly(Config::Command::ID commandID, Config::Command::Action action, ControlGroup::Group group = ControlGroup::Group::Unassinged) {
        try {
            iterateCommandsWithId(commandID, [this, action, group] (QueueCommand* cmd) {
                if (cmd->cmdGroup != group) return;
                addActionForAvailableTypes(cmd, action);
            });
            return true;
        } catch (std::out_of_range e) {
            return false;
        }
    }
    
    bool invoke(Config::Command::ID commandID, bool isPressed = false, bool isLatching = false, ControlGroup::Group group = ControlGroup::Group::Unassinged) {
        
        try {
            iterateCommandsWithId(commandID, [this, group, &isPressed, isLatching] (QueueCommand* cmd) {
                if (cmd->cmdGroup != group) return;

                if (cmd->isPressed && isLatching) isPressed = false;

                if (isPressed && !cmd->isPressed) {
                    cmd->isPressed = true;
                    
                    if (cmd->pressCounter == 0) {
                        cmd->triggerTime = currentTime;
                        cmd->holdTriggerTime = cmd->triggerTime + Config::HoldTimeout;
                        cmd->doublePressTriggerTime = cmd->triggerTime + Config::DoublePressTimeout;
                    }

                    cmd->pressCounter++;
                } else {
                    cmd->isPressed = false;
                }
            });
            return true;
        } catch (std::out_of_range e) {
            return false;
        }
    }

    void process(juce::int64 time, bool isBeat, bool isUpBeat) {
        setCurrentTime(time);
        processCommands();
        processQueue(isBeat, isUpBeat);
    }
    
    
    
private:
    
    void setCurrentTime(juce::int64 time) {
        currentTime = time;
    }

    void handleCommandState(QueueCommand* cmd) {
        if (cmd->isPressed) {
            handleCommandPressState(cmd);
        } else {
            handleCommandReleaseState(cmd);
        }
    }

    void handleCommandPressState(QueueCommand* cmd) {
        checkFunctionModifier(cmd);
        cmd->wasPressed = true;
        cmd->isReleased = false;

        if (!cmd->wasInstantPressed) {
            addActionForAvailableTypes(cmd, Config::Command::Action::InstantPress);
            cmd->wasInstantPressed = true;
        }

        if (cmd->holdTriggerTime > currentTime) return;

        // double hold
        if (!cmd->wasDoubleHeld && cmd->pressCounter > 1) {
            clearQueueByID(cmd->cmdID);
            addActionForAvailableTypes(cmd, Config::Command::Action::DoubleHold);
            cmd->wasDoubleHeld = true;
        }
        
        // hold
        if (!cmd->wasHeld && !cmd->wasDoubleHeld) {
            clearQueueByID(cmd->cmdID);
            addActionForAvailableTypes(cmd, Config::Command::Action::Hold);
            cmd->wasHeld = true;
        }
    }
    
    void handleCommandReleaseState(QueueCommand* cmd) {
        checkFunctionModifier(cmd);

        if (cmd->wasPressed && !cmd->wasDoublePressed && !cmd->wasHeld && !cmd->wasDoubleHeld && cmd->pressCounter > 1) {
            clearQueueByID(cmd->cmdID);
            addActionForAvailableTypes(cmd, Config::Command::Action::DoublePress);
            cmd->wasDoublePressed = true;
        }

        if (cmd->wasPressed && !cmd->wasDoublePressed && !cmd->wasHeld && !cmd->wasDoubleHeld) {
            addActionForAvailableTypes(cmd, Config::Command::Action::Press, Config::DoublePressTimeout);
        }

        if (!cmd->isReleased) {
            addActionForAvailableTypes(cmd, Config::Command::Action::Release);
        }

        cmd->isReleased = true;
        cmd->wasHeld = false;
        cmd->wasDoubleHeld = false;
        cmd->wasInstantPressed = false;
        cmd->wasDoublePressed = false;
        cmd->wasPressed = false;
        
        
    }
    
    void checkFunctionModifier(QueueCommand* cmd) {
        _isFunctionDown = cmd->cmdID == FNCommandID && cmd->isPressed;
    }
    
    void addActionForAvailableTypes(QueueCommand* cmd, Config::Command::Action action, juce::int64 delay = 0) {
        std::map<const Config::Command::Trigger, std::function<void(QueueAction* action)>> actions = cmd->callbacks[action];
        for (auto it = actions.begin(); it != actions.end(); it++) {
            queue.push_back(new QueueAction(cmd->cmdID, currentTime + delay, (Config::Command::Trigger) it->first, actions, _isFunctionDown, cmd->cmdGroup));
        }
    }
    
    void processCommands() {
        for (auto it = commands.begin(); it != commands.end(); it++) {
            QueueCommand* cmd = it->second;
            handleCommandState(cmd);
            decreasePressCounter(cmd);
        }
    }
    
    void decreasePressCounter(QueueCommand* cmd) {
        juce::int64 inDoublePressTimeoutWindow = currentTime - cmd->triggerTime >= Config::DoublePressTimeout;
        if (!cmd->isPressed && inDoublePressTimeoutWindow && cmd->pressCounter > 0) cmd->pressCounter--;
    }
    
    //TODO: maybe check if the trigger times is to close to the next beat time to prevent keypresses to bleed trough
    void processQueue(bool isBeat, bool isUpBeat) {
        for(QueueAction* action: queue) {
            executeUpBeatActions(action, isUpBeat);
            executeBeatActions(action, isBeat);
            executeInstantActions(action);
        }
    }
    
    void executeUpBeatActions(QueueAction* action, bool isUpBeat) {
        if (action->trigger != Config::Command::Trigger::OnUpBeat) return;
        if (!isUpBeat) return;
        if (action->executionTime > currentTime) return;
        if (action->callbacks[action->trigger] == nullptr) return;
        action->callbacks[action->trigger](action);
        removeQueueAction(action);
    }
    
    void executeBeatActions(QueueAction* action, bool isBeat) {
        if (action->trigger != Config::Command::Trigger::OnBeat) return;
        if (!isBeat) return;
        if (action->executionTime > currentTime) return;
        if (action->callbacks[action->trigger] == nullptr) return;
        action->callbacks[action->trigger](action);
        removeQueueAction(action);
    }
    
    void executeInstantActions(QueueAction* action) {
        if (action->trigger != Config::Command::Trigger::Instant) return;
        if (action->executionTime > currentTime) return;
        if (action->callbacks[action->trigger] == nullptr) return;
        action->callbacks[action->trigger](action);
        removeQueueAction(action);
    }
    
    int getCommandIdentifier(Config::Command::ID commandID, ControlGroup::Group group ) {
        return std::stoi(std::to_string((int)commandID) + std::to_string(int(group)));
    }
    
    QueueCommand* getExistingCommand(Config::Command::ID commandID, ControlGroup::Group group = ControlGroup::Group::Unassinged) {
        int cmdIdentity = getCommandIdentifier(commandID, group);

        try {
            return commands.at(cmdIdentity);
        } catch (std::out_of_range e) {
            QueueCommand* cmd = new QueueCommand();
            cmd->cmdID = commandID;
            cmd->cmdGroup = group;
            commands[cmdIdentity] = cmd;

            return commands.at(cmdIdentity);
        }
    }
    
    void iterateCommandsWithId(Config::Command::ID cmdID, std::function<void(QueueCommand* cmd)> callback) {
        for (auto cmd = commands.begin(); cmd != commands.end(); cmd++) {
            QueueCommand* command = cmd->second;
            if (command->cmdID == cmdID) callback(command);
        }
    }
    
    void removeQueueAction(QueueAction* action) {
        queue.erase(std::remove(queue.begin(), queue.end(), action), queue.end());
    }

    void clearQueueByID(Config::Command::ID cmdID) {
        queue.erase(std::remove_if(queue.begin(), queue.end(), [&cmdID](QueueAction* action)->bool {
            return action->cmdID == cmdID;
        }), queue.end());
    }
    
};
