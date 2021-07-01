#pragma once

#include "CommandQueue.h"
#include "../Tracks/Track.h"

class SpoolProcessor;

class Commands {
public:
    using Type = QueueCommand::ActionType;
    using TriggerType = QueueCommand::TriggerType;
    using Cmd = Config::Command::ID;
    using TrackAction = Track::Action;
    using TrackActionMode = Track::ActionMode;
    
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
    void registerControlGroupACommands();
    void registerControlGroupBCommands();
    
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

