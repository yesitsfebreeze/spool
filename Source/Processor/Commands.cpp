#include "Commands.h"

#include "Processor/SpoolProcessor.h"
#include "Processor/Modules/Commands/CommandQueue.h"
#include "Config.h"

void Commands::registerFunctionCommands() {

    // FUNCTION Instant Press
    processor->commandQueue.registerCommand(CmdID::Function, CmdAction::InstantPress, CmdTrigger::Instant, [this] (QueueAction* action) {
        if (processor->isFunctionDown) {
            processor->isFunctionDown = false;
            processor->setEffectMode(false);
        } else {
            processor->isFunctionDown = true;
            processor->setEffectMode(true);
        }
    });

    // FUNCTION Double Press
    processor->commandQueue.registerCommand(CmdID::Function, CmdAction::DoublePress, CmdTrigger::Instant, [this] (QueueAction* action) {
        processor->tracks->doForUnselectedTracks(TrackAction::UnsetGroup);
        processor->tracks->doForAllTracks(TrackAction::Select, TrackMode::Off);

    });

    // FUNCTION Double Hold
    processor->commandQueue.registerCommand(CmdID::Function, CmdAction::DoubleHold, CmdTrigger::Instant, [this] (QueueAction* action) {
        processor->tracks->doForAllTracks(TrackAction::Select, TrackMode::Toggle);
    });
}

void Commands::registerMuteCommands() {
    
    // MUTE Instant Press
    processor->commandQueue.registerCommand(CmdID::Mute, CmdAction::InstantPress, CmdTrigger::Instant, [this] (QueueAction* action) {
        processor->setEffectMode(false);
        processor->isMuteDown = true;
    });

    // MUTE Release
    processor->commandQueue.registerCommand(CmdID::Mute, CmdAction::Release, CmdTrigger::Instant, [this] (QueueAction* action) {
        processor->isMuteDown = false;
    });

    // MUTE Press
    processor->commandQueue.registerCommand(CmdID::Mute, CmdAction::Press, CmdTrigger::OnBeat, [this] (QueueAction* action) {
        if (!action->isFunctionDown) processor->tracks->doForSelectedTracks(TrackAction::Mute, TrackMode::Toggle);
        if (action->isFunctionDown) processor->tracks->doForAllTracks(TrackAction::Mute, TrackMode::Off);
    });

    // MUTE Double Press
    processor->commandQueue.registerCommand(CmdID::Mute, CmdAction::DoublePress, CmdTrigger::OnBeat, [this] (QueueAction* action) {
        processor->tracks->doForAllTracks(TrackAction::Mute, TrackMode::Toggle);
    });

    // MUTE Hold
    processor->commandQueue.registerCommand(CmdID::Mute, CmdAction::Hold, CmdTrigger::Instant, [this] (QueueAction* action) {
        if (!action->isFunctionDown) processor->tracks->doForSelectedTracks(TrackAction::Cue, TrackMode::Toggle);
        if (action->isFunctionDown) processor->tracks->doForAllTracks(TrackAction::Cue, TrackMode::Off);
    });
}

void Commands::registerPlayCommands() {

    // PLAY Instant Press
    processor->commandQueue.registerCommand(CmdID::Play, CmdAction::InstantPress, CmdTrigger::Instant, [this] (QueueAction* action) {
        processor->setEffectMode(false);
        processor->isPlayDown = true;
    });

    // PLAY Release
    processor->commandQueue.registerCommand(CmdID::Play, CmdAction::Release, CmdTrigger::Instant, [this] (QueueAction* action) {
        processor->isPlayDown = false;
    });

    // PLAY Press
    processor->commandQueue.registerCommand(CmdID::Play, CmdAction::Press, CmdTrigger::OnUpBeat, [this] (QueueAction* action) {
        if (!action->isFunctionDown) processor->tracks->doForSelectedTracks(TrackAction::Play, TrackMode::Toggle);
        if (action->isFunctionDown) processor->tracks->doForAllTracks(TrackAction::Play, TrackMode::Off);
    });

    // PLAY Double Press
    processor->commandQueue.registerCommand(CmdID::Play, CmdAction::DoublePress, CmdTrigger::OnUpBeat, [this] (QueueAction* action) {
        if (!action->isFunctionDown) processor->tracks->doForAllTracks(TrackAction::Play, TrackMode::On);
        if (action->isFunctionDown) processor->tracks->doForSelectedTracks(TrackAction::Restart);
    });
}

void Commands::registerRecordCommands() {

    // RECORD Instant Press
    processor->commandQueue.registerCommand(CmdID::Record, CmdAction::InstantPress, CmdTrigger::Instant, [this] (QueueAction* action) {
        processor->setEffectMode(false);
        processor->isRecordDown = true;
    });

    // RECORD Release
    processor->commandQueue.registerCommand(CmdID::Record, CmdAction::Release, CmdTrigger::Instant, [this] (QueueAction* action) {
        processor->isRecordDown = false;
    });

    // RECORD Press
    processor->commandQueue.registerCommand(CmdID::Record, CmdAction::Press, CmdTrigger::Instant, [this] (QueueAction* action) {
        if (!action->isFunctionDown) processor->tracks->doForLastSelectedOrFreeTrack(TrackAction::Record);
    });

    // RECORD Double Press
    processor->commandQueue.registerCommand(CmdID::Record, CmdAction::DoublePress, CmdTrigger::OnBeat, [this] (QueueAction* action) {
        if (!action->isFunctionDown) processor->tracks->doForAllTracks(TrackAction::CancelRecord);
    });

    // RECORD Hold
    processor->commandQueue.registerCommand(CmdID::Record, CmdAction::Hold, CmdTrigger::OnUpBeat, [this] (QueueAction* action) {
        if (!action->isFunctionDown) processor->tracks->doForSelectedTracks(TrackAction::Stop, TrackMode::On);
        if (action->isFunctionDown) processor->tracks->doForAllTracks(TrackAction::Stop, TrackMode::On);
    });

    // RECORD Double Hold
    processor->commandQueue.registerCommand(CmdID::Record, CmdAction::DoubleHold, CmdTrigger::OnUpBeat, [this] (QueueAction* action) {
        if (!action->isFunctionDown) processor->tracks->doForSelectedTracks(TrackAction::Clear);
        if (action->isFunctionDown) processor->tracks->doForAllTracks(TrackAction::Clear);
    });
}



void Commands::registerTrackCommands() {

    int firstTrackEnum = CmdID::Track1;
    for (int track = 0;track < Config::TrackCount; track++) {
        Config::Command::ID trackCmd = static_cast<Config::Command::ID>(track + firstTrackEnum);

        // TRACK Instant Press
        processor->commandQueue.registerCommand(trackCmd, CmdAction::InstantPress, CmdTrigger::Instant, [this, track] (QueueAction* action) {
            processor->tracks->getTrack(track)->setPressed(true);
        });

        // TRACK Release
        processor->commandQueue.registerCommand(trackCmd, CmdAction::Release, CmdTrigger::Instant, [this, track] (QueueAction* action) {
            if (processor->tracks->getTrack(track)->isSelected()) {
                processor->tracks->getTrack(track)->setPressed(false);
            } else {
                juce::Timer::callAfterDelay(Config::DoublePressTimeout, [this, track] {
                    processor->tracks->getTrack(track)->setPressed(false);
                });
            }
        });

        // TRACK Press
        processor->commandQueue.registerCommand(trackCmd, CmdAction::Press, CmdTrigger::Instant, [this, track] (QueueAction* action) {
            if (!action->isFunctionDown) processor->tracks->doForTrack(track, TrackAction::Select, TrackMode::Toggle);
            if (action->isFunctionDown) processor->tracks->doForTrack(track, TrackAction::SelectEffect, TrackMode::Toggle);
        });

        // TRACK Double Press
        processor->commandQueue.registerCommand(trackCmd, CmdAction::DoublePress, CmdTrigger::OnUpBeat, [this, track] (QueueAction* action) {
            if (!action->isFunctionDown) processor->tracks->doForTrack(track, TrackAction::Play, TrackMode::Toggle);
            if (action->isFunctionDown) processor->tracks->doForTrack(track, TrackAction::Stop, TrackMode::Toggle);
        });

        // TRACK Hold
        processor->commandQueue.registerCommand(trackCmd, CmdAction::Hold, CmdTrigger::OnBeat, [this, track] (QueueAction* action) {
            if (!action->isFunctionDown) processor->tracks->doForTrack(track, TrackAction::Mute, TrackMode::Toggle);
            if (action->isFunctionDown) processor->tracks->doForTrack(track, TrackAction::Cue, TrackMode::Toggle);
        });

        // TRACK Double Hold
        processor->commandQueue.registerCommand(trackCmd, CmdAction::DoubleHold, CmdTrigger::Instant, [this, track] (QueueAction* action) {
            if (processor->tracks->hasSampleLayer()) {
                processor->tracks->unsetSampleLayer();
            } else {
                processor->tracks->setSampleLayer(track);
            }
        });
    }
}

void Commands::registerControlGroupCommands(ControlGroup::Group group) {
    
    ControlGroup* controlGroup = processor->controlGroups[group];

    std::function<void(QueueAction* action)> onInteract = [this, group, controlGroup] (QueueAction* action) {
        
        if (processor->isEffectMode()) {
            processor->tracks->doForAllEffects(TrackAction::SetEffectGroup, TrackMode::Off, group);
            processor->tracks->doForSelectedEffects(TrackAction::SetEffectGroup, TrackMode::On, group);
        } else {
            processor->tracks->doForSelectedTracks(TrackAction::SetGroup, TrackMode::On, group);
        }
        
        processor->tracks->doForAllTracks(TrackAction::Select, TrackMode::Off);
        processor->tracks->doForAllTracks(TrackAction::SelectEffect, TrackMode::Off);

        processor->isFunctionDown = false;
        processor->setEffectMode(false);
    };
    
    processor->commandQueue.registerCommand(CmdID::Volume, CmdAction::Interact, CmdTrigger::Instant, onInteract, group);
    processor->commandQueue.registerCommand(CmdID::Wet, CmdAction::Interact, CmdTrigger::Instant, onInteract, group);
    processor->commandQueue.registerCommand(CmdID::ParamA, CmdAction::Interact, CmdTrigger::Instant, onInteract, group);
    processor->commandQueue.registerCommand(CmdID::ParamB, CmdAction::Interact, CmdTrigger::Instant, onInteract, group);

    processor->commandQueue.registerCommand(CmdID::Volume, CmdAction::Increase, CmdTrigger::Instant, [this, controlGroup] (QueueAction* action) {
        controlGroup->doForTracks([this, action] (Track* track) {
            Parameters::get(track->getEnum(), Config::Parameter::Volume).increase(Config::ParamChangePerStep);
        });
    }, group);
    
    processor->commandQueue.registerCommand(CmdID::Volume, CmdAction::Decrease, CmdTrigger::Instant, [this, controlGroup] (QueueAction* action) {
        controlGroup->doForTracks([this, action] (Track* track) {
            Parameters::get(track->getEnum(), Config::Parameter::Volume).decrease(Config::ParamChangePerStep);
        });
    }, group);
    
    processor->commandQueue.registerCommand(CmdID::Balance, CmdAction::Increase, CmdTrigger::Instant, [this, controlGroup] (QueueAction* action) {
        controlGroup->doForTracks([this, action] (Track* track) {
            Parameters::get(track->getEnum(), Config::Parameter::Balance).increase(Config::ParamChangePerStep);
        });
    }, group);
    
    processor->commandQueue.registerCommand(CmdID::Balance, CmdAction::Decrease, CmdTrigger::Instant, [this, controlGroup] (QueueAction* action) {
        controlGroup->doForTracks([this, action] (Track* track) {
            Parameters::get(track->getEnum(), Config::Parameter::Balance).decrease(Config::ParamChangePerStep);
        });
    }, group);
    
    processor->commandQueue.registerCommand(CmdID::Wet, CmdAction::Increase, CmdTrigger::Instant, [this, controlGroup] (QueueAction* action) {
        controlGroup->doForEffects([this, action] (Track* track, Effect* effect) {
            Parameters::get(track->getEnum(), effect->getEnum(), Config::Parameter::Wet).increase(Config::ParamChangePerStep);
        });
    }, group);
    
    processor->commandQueue.registerCommand(CmdID::Wet, CmdAction::Decrease, CmdTrigger::Instant, [this, controlGroup] (QueueAction* action) {
        controlGroup->doForEffects([this, action] (Track* track, Effect* effect) {
            Parameters::get(track->getEnum(), effect->getEnum(), Config::Parameter::Wet).decrease(Config::ParamChangePerStep);
        });
    }, group);
    
    
    processor->commandQueue.registerCommand(CmdID::ParamA, CmdAction::Increase, CmdTrigger::Instant, [this, controlGroup] (QueueAction* action) {
        controlGroup->doForEffects([this, action] (Track* track, Effect* effect) {
            Parameters::get(track->getEnum(), effect->getEnum(), Config::Parameter::ParamA).increase(Config::ParamChangePerStep);
        });
    }, group);
    
    processor->commandQueue.registerCommand(CmdID::ParamA, CmdAction::Decrease, CmdTrigger::Instant, [this, controlGroup] (QueueAction* action) {
        controlGroup->doForEffects([this, action] (Track* track, Effect* effect) {
            Parameters::get(track->getEnum(), effect->getEnum(), Config::Parameter::ParamA).decrease(Config::ParamChangePerStep);
        });
    }, group);
    
    processor->commandQueue.registerCommand(CmdID::ParamB, CmdAction::Increase, CmdTrigger::Instant, [this, controlGroup] (QueueAction* action) {
        controlGroup->doForEffects([this, action] (Track* track, Effect* effect) {
            Parameters::get(track->getEnum(), effect->getEnum(), Config::Parameter::ParamB).increase(Config::ParamChangePerStep);
        });
    }, group);
    
    processor->commandQueue.registerCommand(CmdID::ParamB, CmdAction::Decrease, CmdTrigger::Instant, [this, controlGroup] (QueueAction* action) {
        controlGroup->doForEffects([this, action] (Track* track, Effect* effect) {
            Parameters::get(track->getEnum(), effect->getEnum(), Config::Parameter::ParamB).decrease(Config::ParamChangePerStep);
        });
    }, group);

}
