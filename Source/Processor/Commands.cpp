#include "Commands.h"

#include "Modules/Commands/CommandQueue.h"
#include "SpoolProcessor.h"
#include "../Config.h"


void Commands::registerFunctionCommandActions() {
    
    // FUNCTION Instant Press
    owner->commandQueue.registerCommandAction(Cmd::Function, Type::InstantPress, TriggerType::Instant, [this] (bool FN) {
        DBG("test");
        if (owner->isFunctionDown) {
            owner->isFunctionDown = false;
            owner->setEffectMode(false);
        } else {
            owner->isFunctionDown = true;
            owner->setEffectMode(true);
        }
    });
    
    // FUNCTION Release
    owner->commandQueue.registerCommandAction(Cmd::Function, Type::Release, TriggerType::Instant, [this] (bool FN) {
        //NOTE: give the functionrelease a bit leeway, so ppl can be sloppy with input
        juce::Timer::callAfterDelay(Config::FNResetDelay, [this] {
            owner->isFunctionDown = false;
            owner->setEffectMode(false);
        }); 
    });
    
    // FUNCTION Double Press
    owner->commandQueue.registerCommandAction(Cmd::Function, Type::DoublePress, TriggerType::Instant, [this] (bool FN) {
        owner->tracks->doForUnselectedTracks(TrackAction::RemoveGroup);
        owner->tracks->doForAllTracks(TrackAction::Select, TrackActionMode::Off);
        
    });

    // FUNCTION Double Hold
    owner->commandQueue.registerCommandAction(Cmd::Function, Type::DoubleHold, TriggerType::Instant, [this] (bool FN) {
        owner->tracks->doForAllTracks(TrackAction::Select, TrackActionMode::Toggle);
    });
}

void Commands::registerMuteCommandActions() {

    // MUTE Instant Press
    owner->commandQueue.registerCommandAction(Cmd::Mute, Type::InstantPress, TriggerType::Instant, [this] (bool FN) {
        owner->isMuteDown = true;
    });

    // MUTE Release
    owner->commandQueue.registerCommandAction(Cmd::Mute, Type::Release, TriggerType::Instant, [this] (bool FN) {
        owner->isMuteDown = false;
    });
    
    // MUTE Press
    owner->commandQueue.registerCommandAction(Cmd::Mute, Type::Press, TriggerType::OnBeat, [this] (bool FN) {
        if (!FN) owner->tracks->doForSelectedTracks(TrackAction::Mute, TrackActionMode::Toggle);
        if (FN)  owner->tracks->doForAllTracks(TrackAction::Mute, TrackActionMode::Off);
    });

    // MUTE Double Press
    owner->commandQueue.registerCommandAction(Cmd::Mute, Type::DoublePress, TriggerType::OnBeat, [this] (bool FN) {
        owner->tracks->doForAllTracks(TrackAction::Mute, TrackActionMode::Toggle);
    });

    // MUTE Hold
    owner->commandQueue.registerCommandAction(Cmd::Mute, Type::Hold, TriggerType::Instant, [this] (bool FN) {
        if (!FN) owner->tracks->doForSelectedTracks(TrackAction::Cue, TrackActionMode::Toggle);
        if (FN)  owner->tracks->doForAllTracks(TrackAction::Cue, TrackActionMode::Off);
    });
}

void Commands::registerPlayCommandActions() {

    // PLAY Instant Press
    owner->commandQueue.registerCommandAction(Cmd::Play, Type::InstantPress, TriggerType::Instant, [this] (bool FN) {
        owner->isPlayDown = true;
    });

    // PLAY Release
    owner->commandQueue.registerCommandAction(Cmd::Play, Type::Release, TriggerType::Instant, [this] (bool FN) {
        owner->isPlayDown = false;
    });
    
    // PLAY Press
    owner->commandQueue.registerCommandAction(Cmd::Play, Type::Press, TriggerType::OnUpBeat, [this] (bool FN) {
        if (!FN) owner->tracks->doForSelectedTracks(TrackAction::Play, TrackActionMode::Toggle);
        if (FN)  owner->tracks->doForAllTracks(TrackAction::Play, TrackActionMode::Off);
    });

    // PLAY Double Press
    owner->commandQueue.registerCommandAction(Cmd::Play, Type::DoublePress, TriggerType::OnUpBeat, [this] (bool FN) {
        if (!FN) owner->tracks->doForAllTracks(TrackAction::Play, TrackActionMode::On);
        if (FN) owner->tracks->doForSelectedTracks(TrackAction::Restart);
    });
}

void Commands::registerRecordCommandActions() {

    // RECORD Instant Press
    owner->commandQueue.registerCommandAction(Cmd::Record, Type::InstantPress, TriggerType::Instant, [this] (bool FN) {
        owner->isRecordDown = true;
    });

    // RECORD Release
    owner->commandQueue.registerCommandAction(Cmd::Record, Type::Release, TriggerType::Instant, [this] (bool FN) {
        owner->isRecordDown = false;
    });
    
    // RECORD Press
    owner->commandQueue.registerCommandAction(Cmd::Record, Type::Press, TriggerType::Instant, [this] (bool FN) {
        if (!FN) owner->tracks->doForLastSelectedOrFreeTrack(TrackAction::Record);
    });

    // RECORD Double Press
    owner->commandQueue.registerCommandAction(Cmd::Record, Type::DoublePress, TriggerType::OnBeat, [this] (bool FN) {
        if (!FN) owner->tracks->doForAllTracks(TrackAction::CancelRecord);
    });

    // RECORD Hold
    owner->commandQueue.registerCommandAction(Cmd::Record, Type::Hold, TriggerType::OnUpBeat, [this] (bool FN) {
        if (!FN) owner->tracks->doForSelectedTracks(TrackAction::Stop, TrackActionMode::On);
        if (FN)  owner->tracks->doForAllTracks(TrackAction::Stop, TrackActionMode::On);
    });
    
    // RECORD Double Hold
    owner->commandQueue.registerCommandAction(Cmd::Record, Type::DoubleHold, TriggerType::OnUpBeat, [this] (bool FN) {
        if (!FN) owner->tracks->doForSelectedTracks(TrackAction::Clear);
        if (FN) owner->tracks->doForAllTracks(TrackAction::Clear);
    });
}



void Commands::registerTrackCommandActions() {

    int firstTrackEnum = Cmd::Track1;
    for (int track = 0;track < Config::TrackCount; track++) {
        Cmd trackCmd = static_cast<Cmd>(track + firstTrackEnum);

        // TRACK Instant Press
        owner->commandQueue.registerCommandAction(trackCmd, Type::InstantPress, TriggerType::Instant, [this, track] (bool FN) {
            owner->tracks->getTrack(track)->setPressed(true);
        });

        // TRACK Release
        owner->commandQueue.registerCommandAction(trackCmd, Type::Release, TriggerType::Instant, [this, track] (bool FN) {
            juce::Timer::callAfterDelay(Config::TrackResetDelay, [this, track] { owner->tracks->getTrack(track)->setPressed(false); });
        });
        
        // TRACK Press
        owner->commandQueue.registerCommandAction(trackCmd, Type::Press, TriggerType::Instant, [this, track] (bool FN) {
            if (!FN) owner->tracks->doForTrack(track, TrackAction::Select, TrackActionMode::Toggle);
//            if (FN)  owner->effects.doForTrack(track, Effects::Type::Select, Effects::Action::Toggle);
        });
        
        // TRACK Double Press
        owner->commandQueue.registerCommandAction(trackCmd, Type::DoublePress, TriggerType::OnUpBeat, [this, track] (bool FN) {
            if (!FN) owner->tracks->doForTrack(track, TrackAction::Play, TrackActionMode::Toggle);
            if (FN)  owner->tracks->doForTrack(track, TrackAction::Stop, TrackActionMode::Toggle);
        });

        // TRACK Hold
        owner->commandQueue.registerCommandAction(trackCmd, Type::Hold, TriggerType::OnBeat, [this, track] (bool FN) {
            if (!FN) owner->tracks->doForTrack(track, TrackAction::Mute, TrackActionMode::Toggle);
            if (FN)  owner->tracks->doForTrack(track, TrackAction::Cue, TrackActionMode::Toggle);
        });
        
        // TRACK Double Hold
        owner->commandQueue.registerCommandAction(trackCmd, Type::DoubleHold, TriggerType::Instant, [this, track] (bool FN) {
            if (owner->tracks->hasOverdubLayer()) {
                owner->tracks->unsetOverdubLayer();
            } else {
                owner->tracks->setOverdubLayer(track);
            }
        });

    }

}
