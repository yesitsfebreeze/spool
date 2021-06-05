#pragma once
#include <JuceHeader.h>
#include <map>
#include "../../Config.h"

struct QueueCommand {
public:
    enum ActionType {
        Press,
        InstantPress,
        Release,
        Hold,
        DoublePress,
        DoubleHold,
        
        //TODO: implement ?
        Debounce,
        BeatDebounce
    };
    
    enum TriggerType {
        Instant,
        OnBeat,
        OnUpBeat
    };
    
    ActionType actionType;
    TriggerType triggerType;
    Config::Command::ID cmdID;
    
    std::function<void(bool withFN)> onPress;
    TriggerType pressTriggerType = TriggerType::Instant;

    std::function<void(bool withFN)> onInstantPress;
    std::function<void(bool withFN)> onRelease;
        
    std::function<void(bool withFN)> onHold;
    TriggerType holdTriggerType = TriggerType::Instant;

    std::function<void(bool withFN)> onDoublePress;
    TriggerType doublePressTriggerType = TriggerType::Instant;
    
    std::function<void(bool withFN)> onDoubleHold;
    TriggerType doubleHoldTriggerType = TriggerType::Instant;

    //TODO: implement ?
    std::function<void(bool withFN)> onDebounce;
    std::function<void(bool withFN)> onBeatDebounce;

    bool isPressed = false;
    bool isReleased = true;
    bool wasPressed = false;
    bool wasDoublePressed = false;
    bool wasDoubleHeld = false;
    bool wasHeld = false;
    
    int pressCounter = 0;
    juce::int64 triggerTime = 0;
    
    bool clearedPress = false;
    bool clearedDoublePress = false;

    int debounceTimeout = Config::debounceTimeout;
    int debounceBeats = 1;
    
};


struct QueueAction {
public:
    QueueAction(
        Config::Command::ID cmdID,
        juce::int64 executionTime,
        QueueCommand::TriggerType triggerType,
        std::function<void(bool withFN)> callback,
        bool isFunctionDown = false
    ) :
    cmdID(cmdID),
    executionTime(executionTime),
    triggerType(triggerType),
    callback(callback),
    isFunctionDown(isFunctionDown) {}

    Config::Command::ID cmdID;
    juce::int64 executionTime = 0;
    QueueCommand::TriggerType triggerType = QueueCommand::TriggerType::Instant;
    std::function<void(bool withFN)> callback;
    bool isFunctionDown = false;
};


class CommandQueue {
public:

    bool isFunctionDown = false;
    bool isMuteDown = false;
    bool isPlayDown = false;
    bool isRecordDown = false;
    
    bool canExecuteFnFunctions = true;
    juce::int64 currentTime = 0;
    Config::Command::ID FNCommandID;
    
    std::vector<QueueAction*> queue;
    std::map<const int, QueueCommand*> commands = {};

    void registerCommandAction(Config::Command::ID commandID, QueueCommand::ActionType actionType, QueueCommand::TriggerType triggerType, std::function<void(bool withFN)> callback) {
        
        int cmdIdentity = getCommandIdentifier(commandID, actionType, triggerType);

        try {
            commands.at(cmdIdentity);
        } catch (std::out_of_range e) {
            QueueCommand* cmd = new QueueCommand();
            cmd->actionType = actionType;
            cmd->triggerType = triggerType;
            cmd->cmdID = commandID;
            commands[cmdIdentity] = cmd;
        }

        QueueCommand* cmd = commands.at(cmdIdentity);
        
        switch (actionType) {
            case QueueCommand::ActionType::Press:
                cmd->onPress = callback;
                cmd->pressTriggerType = triggerType;
                break;
            case QueueCommand::ActionType::InstantPress:
                cmd->onInstantPress = callback;
                break;
            case QueueCommand::ActionType::Release:
                cmd->onRelease = callback;
                break;
            case QueueCommand::ActionType::Hold:
                cmd->onHold = callback;
                cmd->holdTriggerType = triggerType;
                break;
            case QueueCommand::ActionType::DoublePress:
                cmd->onDoublePress = callback;
                cmd->doublePressTriggerType = triggerType;
                break;
            case QueueCommand::ActionType::DoubleHold:
                cmd->onDoubleHold = callback;
                cmd->doubleHoldTriggerType = triggerType;
                break;
            case QueueCommand::ActionType::Debounce:
                cmd->onDebounce = callback;
                break;
            case QueueCommand::ActionType::BeatDebounce:
                cmd->onBeatDebounce = callback;
                break;
            default:
                break;
        }
    };
    
    
    bool triggerCommand(Config::Command::ID commandID, bool isPressed) {
        try {
            executeForCommandsWithID(commandID, [this, isPressed] (QueueCommand* command) {
                if (isPressed && !command->isPressed) {
                    command->isPressed = true;
                    command->triggerTime = currentTime;
                    command->pressCounter++;
                }

                if (!isPressed && command->isPressed) {
                    command->isPressed = false;
                }
            });
            
            return true;
        } catch (std::out_of_range e) {
            return false;
        }        
    }

    void process(bool isBeat, bool isUpBeat) {
        processCommands(currentTime);
        processActions(currentTime, isBeat, isUpBeat);
    }
    
    void setCurrentTime(juce::int64 time) {
        currentTime = time;
    }
    
private:
    
    int getCommandIdentifier(Config::Command::ID commandID, QueueCommand::ActionType type, QueueCommand::TriggerType triggerType) {
        return std::stoi(std::to_string((int)commandID) + std::to_string(int(type)) + std::to_string(int(triggerType)));
    }
    
    void handleCommandState(QueueCommand* command, juce::int64 currentTime) {
        juce::int64 holdTimeoutTarget = command->triggerTime + Config::holdTimeout;
        
        bool FN = isFunctionDown || command->cmdID == FNCommandID;
        
        if (command->isPressed) {
            command->isReleased = false;
            
            if (command->cmdID != FNCommandID) {
                canExecuteFnFunctions = false;
            } else if (!canExecuteFnFunctions) {
                return;
            }

            if (!command->wasDoublePressed && command->pressCounter > 1) {
                if (command->onInstantPress != nullptr) command->onInstantPress(FN);
                clearQueueAction(command->cmdID);
                command->wasDoublePressed = true;
                return;
            }

            if (!command->wasPressed && !command->wasDoublePressed) {
                if (command->onInstantPress != nullptr) command->onInstantPress(FN);
                clearQueueAction(command->cmdID);
                command->wasPressed = true;
                return;
            }

            if (holdTimeoutTarget <= currentTime) {
                if (command->wasDoublePressed && !command->wasDoubleHeld) {
                    command->wasDoubleHeld = true;
                    if (command->actionType == QueueCommand::ActionType::DoubleHold) {
                        QueueAction* action = new QueueAction(command->cmdID, currentTime, command->doubleHoldTriggerType, command->onDoubleHold, FN);
                        queue.push_back(action);
                    }
                    return;
                }
                if (!command->wasHeld && !command->wasDoubleHeld) {
                    command->wasHeld = true;
                    if (command->actionType == QueueCommand::ActionType::Hold) {
                        QueueAction* action = new QueueAction(command->cmdID, currentTime, command->holdTriggerType, command->onHold, FN);
                        queue.push_back(action);
                    }
                    return;
                }
            }
        } else {
            command->clearedPress = false;
            command->clearedDoublePress = false;
            
            if (!command->isReleased) {
                if (command->onRelease != nullptr) command->onRelease(isFunctionDown);
                command->isReleased = true;
            };
            
            if (command->cmdID == FNCommandID && !canExecuteFnFunctions) {
                canExecuteFnFunctions = true;
                return;
            }
            
            if (command->wasDoubleHeld) {
                command->wasPressed = false;
                command->wasDoublePressed = false;
                command->wasDoubleHeld = false;
                return;
            }

            if (command->wasHeld) {
                command->wasPressed = false;
                command->wasHeld = false;
                return;
            }
            
            if (command->wasDoublePressed && !command->wasDoubleHeld) {
                if (command->actionType == QueueCommand::ActionType::DoublePress) {
                    QueueAction* action = new QueueAction(command->cmdID, command->triggerTime, command->doublePressTriggerType, command->onDoublePress, FN);
                    queue.push_back(action);
                }
                command->wasDoublePressed = false;
                command->wasPressed = false;
                return;
            };
            
            if (command->wasPressed) {
                if (command->actionType == QueueCommand::ActionType::Press) {
                    QueueAction* action = new QueueAction(command->cmdID, command->triggerTime + Config::doublePressTimeWindow, command->pressTriggerType, command->onPress, FN);
                    queue.push_back(action);
                }
                command->wasPressed = false;
            }
        }
    }

    
    void processCommands(juce::int64 currentTime) {
        for (auto it = commands.begin(); it != commands.end(); it++) {
            QueueCommand* command = it->second;
            currentTime = juce::Time::getCurrentTime().toMilliseconds();
            handleCommandState(command, currentTime);
            
            juce::int64 inDoublePressTimeoutWindow = currentTime - command->triggerTime >= Config::doublePressTimeWindow;
            if (!command->isPressed && inDoublePressTimeoutWindow && command->pressCounter > 0) {
                command->pressCounter--;
            }
        }
    }
    
    void processActions(juce::int64 currentTime, bool isBeat, bool isUpBeat) {
        
        //TODO: maybe check if the trigger times is to close to the next beat time to prevent keypresses to bleed trough
        
        for(QueueAction* action: queue) {
            bool canExecute = action->executionTime <= currentTime;
            
            if (action->triggerType == QueueCommand::TriggerType::OnUpBeat) {
                if (isUpBeat && canExecute) {
                    if (action->callback != nullptr) action->callback(action->isFunctionDown);
                    removeQueueAction(action);
                }
            } else if (action->triggerType == QueueCommand::TriggerType::OnBeat) {
                if (isBeat && canExecute) {
                    if (action->callback != nullptr) action->callback(action->isFunctionDown);
                    removeQueueAction(action);
                }
            } else if (action->triggerType == QueueCommand::TriggerType::Instant) {
                if (canExecute) {
                    if (action->callback != nullptr) action->callback(action->isFunctionDown);
                    removeQueueAction(action);
                }
            }
        }
    }
    
    void executeForCommandsWithID(Config::Command::ID cmdID, std::function<void(QueueCommand* command)> cb) {
        for (auto it = commands.begin(); it != commands.end(); it++) {
            QueueCommand* command = it->second;
            if (command->cmdID == cmdID) {
                cb(command);
            }
        }
    }
    
    void removeQueueAction(QueueAction* action) {
        queue.erase(std::remove(queue.begin(), queue.end(), action), queue.end());
    }
    
    void clearQueueAction(Config::Command::ID cmdID) {
        queue.erase(std::remove_if(queue.begin(), queue.end(), [&cmdID](QueueAction* action)->bool {
            return action->cmdID == cmdID;
        }), queue.end());
    }
};
