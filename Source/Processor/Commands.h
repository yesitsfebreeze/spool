#pragma once

#include "Modules/Commands/CommandQueue.h"
#include "Modules/Commands/CommandTypes.h"
#include "Modules/Tracks/Track.h"

class SpoolProcessor;

class Commands {
public:
    using Type = CommandTypes::Action;
    using TriggerType = CommandTypes::Trigger;
    using Cmd = Config::Command::ID;
    using TrackAction = Track::Action;
    using TrackActionMode = Track::ActionMode;
    
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

