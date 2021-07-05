#include "Commands.h"

#include "SpoolProcessor.h"
#include "Modules/Commands/CommandQueue.h"
#include "../Config.h"


void Commands::registerFunctionCommands() {

    // FUNCTION Instant Press
    processor->commandQueue.registerCommand(Cmd::Function, Type::InstantPress, TriggerType::Instant, [this] (QueueAction* action) {
        if (processor->isFunctionDown) {
            processor->isFunctionDown = false;
            processor->setEffectMode(false);
        } else {
            processor->isFunctionDown = true;
            processor->setEffectMode(true);
        }
    });

    // FUNCTION Release
    processor->commandQueue.registerCommand(Cmd::Function, Type::Release, TriggerType::Instant, [this] (QueueAction* action) {
        //NOTE: give the functionrelease a bit leeway, so ppl can be sloppy with input
        juce::Timer::callAfterDelay(Config::FNResetDelay, [this] {
            processor->isFunctionDown = false;
            processor->setEffectMode(false);
        });
    });

    // FUNCTION Double Press
    processor->commandQueue.registerCommand(Cmd::Function, Type::DoublePress, TriggerType::Instant, [this] (QueueAction* action) {
        processor->tracks->doForUnselectedTracks(TrackAction::RemoveTrackFromAllGroups);
        processor->tracks->doForAllTracks(TrackAction::Select, TrackActionMode::Off);

    });

    // FUNCTION Double Hold
    processor->commandQueue.registerCommand(Cmd::Function, Type::DoubleHold, TriggerType::Instant, [this] (QueueAction* action) {
        processor->tracks->doForAllTracks(TrackAction::Select, TrackActionMode::Toggle);
    });
}

void Commands::registerMuteCommands() {
    
    // MUTE Instant Press
    processor->commandQueue.registerCommand(Cmd::Mute, Type::InstantPress, TriggerType::Instant, [this] (QueueAction* action) {
        processor->setEffectMode(false);
        processor->isMuteDown = true;
    });

    // MUTE Release
    processor->commandQueue.registerCommand(Cmd::Mute, Type::Release, TriggerType::Instant, [this] (QueueAction* action) {
        processor->isMuteDown = false;
    });

    // MUTE Press
    processor->commandQueue.registerCommand(Cmd::Mute, Type::Press, TriggerType::OnBeat, [this] (QueueAction* action) {
        if (!action->isFunctionDown) processor->tracks->doForSelectedTracks(TrackAction::Mute, TrackActionMode::Toggle);
        if (action->isFunctionDown) processor->tracks->doForAllTracks(TrackAction::Mute, TrackActionMode::Off);
    });

    // MUTE Double Press
    processor->commandQueue.registerCommand(Cmd::Mute, Type::DoublePress, TriggerType::OnBeat, [this] (QueueAction* action) {
        processor->tracks->doForAllTracks(TrackAction::Mute, TrackActionMode::Toggle);
    });

    // MUTE Hold
    processor->commandQueue.registerCommand(Cmd::Mute, Type::Hold, TriggerType::Instant, [this] (QueueAction* action) {
        if (!action->isFunctionDown) processor->tracks->doForSelectedTracks(TrackAction::Cue, TrackActionMode::Toggle);
        if (action->isFunctionDown) processor->tracks->doForAllTracks(TrackAction::Cue, TrackActionMode::Off);
    });
}

void Commands::registerPlayCommands() {

    // PLAY Instant Press
    processor->commandQueue.registerCommand(Cmd::Play, Type::InstantPress, TriggerType::Instant, [this] (QueueAction* action) {
        processor->setEffectMode(false);
        processor->isPlayDown = true;
    });

    // PLAY Release
    processor->commandQueue.registerCommand(Cmd::Play, Type::Release, TriggerType::Instant, [this] (QueueAction* action) {
        processor->isPlayDown = false;
    });

    // PLAY Press
    processor->commandQueue.registerCommand(Cmd::Play, Type::Press, TriggerType::OnUpBeat, [this] (QueueAction* action) {
        if (!action->isFunctionDown) processor->tracks->doForSelectedTracks(TrackAction::Play, TrackActionMode::Toggle);
        if (action->isFunctionDown) processor->tracks->doForAllTracks(TrackAction::Play, TrackActionMode::Off);
    });

    // PLAY Double Press
    processor->commandQueue.registerCommand(Cmd::Play, Type::DoublePress, TriggerType::OnUpBeat, [this] (QueueAction* action) {
        if (!action->isFunctionDown) processor->tracks->doForAllTracks(TrackAction::Play, TrackActionMode::On);
        if (action->isFunctionDown) processor->tracks->doForSelectedTracks(TrackAction::Restart);
    });
}

void Commands::registerRecordCommands() {

    // RECORD Instant Press
    processor->commandQueue.registerCommand(Cmd::Record, Type::InstantPress, TriggerType::Instant, [this] (QueueAction* action) {
        processor->setEffectMode(false);
        processor->isRecordDown = true;
    });

    // RECORD Release
    processor->commandQueue.registerCommand(Cmd::Record, Type::Release, TriggerType::Instant, [this] (QueueAction* action) {
        processor->isRecordDown = false;
    });

    // RECORD Press
    processor->commandQueue.registerCommand(Cmd::Record, Type::Press, TriggerType::Instant, [this] (QueueAction* action) {
        if (!action->isFunctionDown) processor->tracks->doForLastSelectedOrFreeTrack(TrackAction::Record);
    });

    // RECORD Double Press
    processor->commandQueue.registerCommand(Cmd::Record, Type::DoublePress, TriggerType::OnBeat, [this] (QueueAction* action) {
        if (!action->isFunctionDown) processor->tracks->doForAllTracks(TrackAction::CancelRecord);
    });

    // RECORD Hold
    processor->commandQueue.registerCommand(Cmd::Record, Type::Hold, TriggerType::OnUpBeat, [this] (QueueAction* action) {
        if (!action->isFunctionDown) processor->tracks->doForSelectedTracks(TrackAction::Stop, TrackActionMode::On);
        if (action->isFunctionDown) processor->tracks->doForAllTracks(TrackAction::Stop, TrackActionMode::On);
    });

    // RECORD Double Hold
    processor->commandQueue.registerCommand(Cmd::Record, Type::DoubleHold, TriggerType::OnUpBeat, [this] (QueueAction* action) {
        if (!action->isFunctionDown) processor->tracks->doForSelectedTracks(TrackAction::Clear);
        if (action->isFunctionDown) processor->tracks->doForAllTracks(TrackAction::Clear);
    });
}



void Commands::registerTrackCommands() {

    int firstTrackEnum = Cmd::Track1;
    for (int track = 0;track < Config::TrackCount; track++) {
        Cmd trackCmd = static_cast<Cmd>(track + firstTrackEnum);

        // TRACK Instant Press
        processor->commandQueue.registerCommand(trackCmd, Type::InstantPress, TriggerType::Instant, [this, track] (QueueAction* action) {
            processor->tracks->getTrack(track)->setPressed(true);
        });

        // TRACK Release
        processor->commandQueue.registerCommand(trackCmd, Type::Release, TriggerType::Instant, [this, track] (QueueAction* action) {
            if (processor->tracks->getTrack(track)->isSelected()) {
                processor->tracks->getTrack(track)->setPressed(false);
            } else {
                juce::Timer::callAfterDelay(Config::DoublePressTimeout, [this, track] {
                    processor->tracks->getTrack(track)->setPressed(false);
                });
            }
        });

        // TRACK Press
        processor->commandQueue.registerCommand(trackCmd, Type::Press, TriggerType::Instant, [this, track] (QueueAction* action) {
            if (!action->isFunctionDown) processor->tracks->doForTrack(track, TrackAction::Select, TrackActionMode::Toggle);
            if (action->isFunctionDown) processor->tracks->doForTrack(track, TrackAction::SelectEffect, TrackActionMode::Toggle);
        });

        // TRACK Double Press
        processor->commandQueue.registerCommand(trackCmd, Type::DoublePress, TriggerType::OnUpBeat, [this, track] (QueueAction* action) {
            if (!action->isFunctionDown) processor->tracks->doForTrack(track, TrackAction::Play, TrackActionMode::Toggle);
            if (action->isFunctionDown) processor->tracks->doForTrack(track, TrackAction::Stop, TrackActionMode::Toggle);
        });

        // TRACK Hold
        processor->commandQueue.registerCommand(trackCmd, Type::Hold, TriggerType::OnBeat, [this, track] (QueueAction* action) {
            if (!action->isFunctionDown) processor->tracks->doForTrack(track, TrackAction::Mute, TrackActionMode::Toggle);
            if (action->isFunctionDown) processor->tracks->doForTrack(track, TrackAction::Cue, TrackActionMode::Toggle);
        });

        // TRACK Double Hold
        processor->commandQueue.registerCommand(trackCmd, Type::DoubleHold, TriggerType::Instant, [this, track] (QueueAction* action) {
            if (processor->tracks->hasSampleLayer()) {
                processor->tracks->unsetSampleLayer();
            } else {
                processor->tracks->setSampleLayer(track);
            }
        });
    }
}

void Commands::registerControlGroupCommands(ControlGroup::Group group) {
    
    ControlGroup* processorGroup = &processor->controlGroupA;
    if (group == ControlGroup::Group::B) {
        processorGroup = &processor->controlGroupB;
    }

    std::function<void(QueueAction* action)> onInteract = [this, group] (QueueAction* action) {
        
        Track::Action removeTrackAction = Track::Action::RemoveTrackFromGroupB;
        Track::Action removeEffectAction = Track::Action::RemoveEffectFromGroupB;
        Track::Action addTrackAction = Track::Action::AddTrackToGroupA;
        Track::Action addEffectAction = Track::Action::AddEffectToGroupA;
        
        if (group == ControlGroup::Group::B) {
            removeTrackAction = Track::Action::RemoveTrackFromGroupA;
            removeEffectAction = Track::Action::RemoveEffectFromGroupA;
            addTrackAction = Track::Action::AddTrackToGroupB;
            addEffectAction = Track::Action::AddEffectToGroupB;
        }
        
        if (processor->isEffectMode()) {
            processor->tracks->doForAllTracks(removeTrackAction);
            processor->tracks->doForSelectedTracks(addEffectAction);
        } else {
            processor->tracks->doForSelectedTracks(removeTrackAction);
            processor->tracks->doForSelectedTracks(addTrackAction);
        }

        processor->tracks->doForAllTracks(Track::Action::Select, Track::ActionMode::Off);
        processor->tracks->doForAllTracks(Track::Action::SelectEffect, Track::ActionMode::Off);

        processor->isFunctionDown = false;
        processor->setEffectMode(false);
    };
    
    processor->commandQueue.registerCommand(Cmd::Volume, Type::Interact, TriggerType::Instant, onInteract, group);
    processor->commandQueue.registerCommand(Cmd::Wet, Type::Interact, TriggerType::Instant, onInteract, group);
    processor->commandQueue.registerCommand(Cmd::ParamA, Type::Interact, TriggerType::Instant, onInteract, group);
    processor->commandQueue.registerCommand(Cmd::ParamB, Type::Interact, TriggerType::Instant, onInteract, group);
    
    processor->commandQueue.registerCommand(Cmd::Volume, Type::IncreaseValue, TriggerType::Instant, [this, processorGroup] (QueueAction* action) {
        processorGroup->doForTracks([this, action] (Track* track) {
            Parameters::increaseTrackParam(track->getIndex(), Config::Parameters::Volume);
        });
    }, group);
    
    processor->commandQueue.registerCommand(Cmd::Volume, Type::DecreaseValue, TriggerType::Instant, [this, processorGroup] (QueueAction* action) {
        processorGroup->doForTracks([this, action] (Track* track) {
            Parameters::decreaseTrackParam(track->getIndex(), Config::Parameters::Volume);
        });
    }, group);
    
    processor->commandQueue.registerCommand(Cmd::Wet, Type::IncreaseValue, TriggerType::Instant, [this, processorGroup] (QueueAction* action) {
        processorGroup->doForEffects([this, action] (Track* track, Effect* effect) {
            Parameters::increaseTrackEffectParam(track->getIndex(), effect->index, Config::Parameters::Wet);
        });
    }, group);
    
    processor->commandQueue.registerCommand(Cmd::Wet, Type::DecreaseValue, TriggerType::Instant, [this, processorGroup] (QueueAction* action) {
        processorGroup->doForEffects([this, action] (Track* track, Effect* effect) {
            Parameters::decreaseTrackEffectParam(track->getIndex(), effect->index, Config::Parameters::Wet);
        });
    }, group);
    
    
    processor->commandQueue.registerCommand(Cmd::ParamA, Type::IncreaseValue, TriggerType::Instant, [this, processorGroup] (QueueAction* action) {
        processorGroup->doForEffects([this, action] (Track* track, Effect* effect) {
            Parameters::increaseTrackEffectParam(track->getIndex(), effect->index, Config::Parameters::ParamA);
        });
    }, group);
    
    processor->commandQueue.registerCommand(Cmd::ParamA, Type::DecreaseValue, TriggerType::Instant, [this, processorGroup] (QueueAction* action) {
        processorGroup->doForEffects([this, action] (Track* track, Effect* effect) {
            Parameters::decreaseTrackEffectParam(track->getIndex(), effect->index, Config::Parameters::ParamA);
        });
    }, group);
    
    processor->commandQueue.registerCommand(Cmd::ParamB, Type::IncreaseValue, TriggerType::Instant, [this, processorGroup] (QueueAction* action) {
        processorGroup->doForEffects([this, action] (Track* track, Effect* effect) {
            Parameters::increaseTrackEffectParam(track->getIndex(), effect->index, Config::Parameters::ParamB);
        });
    }, group);
    
    processor->commandQueue.registerCommand(Cmd::ParamB, Type::DecreaseValue, TriggerType::Instant, [this, processorGroup] (QueueAction* action) {
        processorGroup->doForEffects([this, action] (Track* track, Effect* effect) {
            Parameters::decreaseTrackEffectParam(track->getIndex(), effect->index, Config::Parameters::ParamB);
        });
    }, group);

}
