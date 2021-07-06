#pragma once

#include "Processor/Modules/Commands/CommandQueue.h"
#include "Processor/Modules/Commands/CommandTypes.h"
#include "Processor/Modules/Tracks/Track.h"

class SpoolProcessor;

class Commands {
public:

    using CmdID = Config::Command::ID;
    using CmdAction = Config::Command::Action;
    using CmdTrigger = Config::Command::Trigger;
    
    using TrackAction = Track::Action;
    using TrackMode = Track::Mode;
    
    void setProcessor(SpoolProcessor* processor) {
        this->processor = processor;
        registerCommands();
    }
    
    void registerFunctionCommands();
    void registerMuteCommands();
    void registerPlayCommands();
    void registerRecordCommands();
    void registerTrackCommands();
    void registerControlGroupCommands(ControlGroup::Group group);
    
    void registerCommands() {
        registerFunctionCommands();
        registerMuteCommands();
        registerPlayCommands();
        registerRecordCommands();
        registerTrackCommands();
        registerControlGroupCommands(ControlGroup::Group::A);
        registerControlGroupCommands(ControlGroup::Group::B);
    };

private:
    SpoolProcessor* processor;
};

