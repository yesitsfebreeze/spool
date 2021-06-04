#pragma once

#include "Modules/CommandQueue.h"

class SpoolProcessor;

class Commands {
public:
    using Type = QueueCommand::ActionType;
    using TriggerType = QueueCommand::TriggerType;
    using Cmd = Config::Command::ID;
    
    bool isFunctionDown = false;
    
    const bool playOnBeat = true;
    const bool dontPlayOnBeat = false;
    const bool playOnAllBeats = false;
    const bool playOnUpBeatOnly = true;
    
    void setOwner(SpoolProcessor* owner) {
        this->owner = owner;
        
        registerCommandActions();
    }
    
    void registerFunctionCommandActions();
    void registerMuteCommandActions();
    void registerPlayCommandActions();
    void registerRecordCommandActions();
    void registerTrackCommandActions();
    
    void registerCommandActions() {
        registerFunctionCommandActions();
        registerMuteCommandActions();
        registerPlayCommandActions();
        registerRecordCommandActions();
        registerTrackCommandActions();
    };
private:
    SpoolProcessor* owner;
};
