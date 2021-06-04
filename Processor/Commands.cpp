#include "Commands.h"

#include "Modules/CommandQueue.h"
#include "SpoolProcessor.h"
#include "../Config.h"


void Commands::registerFunctionCommandActions() {
    owner->commandQueue.registerCommandAction(Cmd::Function, Type::InstantPress, TriggerType::Instant, [this] (bool FN) {
        owner->commandQueue.isFunctionDown = true;
    });

    owner->commandQueue.registerCommandAction(Cmd::Function, Type::Release, TriggerType::Instant, [this] (bool FN) {
        //NOTE: give the functionrelease a bit leeway, so ppl can be sloppy with input
        juce::Timer::callAfterDelay (100, [this] {owner->commandQueue.isFunctionDown = false;});
    });

    owner->commandQueue.registerCommandAction(Cmd::Function, Type::Hold, TriggerType::Instant, [this] (bool FN) {
        owner->commandQueue.isFunctionDown = true;
        owner->tracks.all(Tracks::Type::Select, Tracks::Action::Off);
    });
    
    owner->commandQueue.registerCommandAction(Cmd::Function, Type::DoublePress, TriggerType::Instant, [this] (bool FN) {
        owner->commandQueue.isFunctionDown = true;
        owner->tracks.selection(Tracks::Type::Select, Tracks::Action::Invert);
    });

    owner->commandQueue.registerCommandAction(Cmd::Function, Type::DoubleHold, TriggerType::Instant, [this] (bool FN) {
        owner->commandQueue.isFunctionDown = true;
        owner->tracks.all(Tracks::Type::Select, Tracks::Action::On);
    });
}

void Commands::registerMuteCommandActions() {
    owner->commandQueue.registerCommandAction(Cmd::Mute, Type::Press, TriggerType::OnUpBeat, [this] (bool FN) {
        DBG("mute: press" << (FN ? " FN" : ""));
        //TODO: implement mute press
        //TODO: implement fn mute press
    });

    owner->commandQueue.registerCommandAction(Cmd::Mute, Type::Hold, TriggerType::OnUpBeat, [this] (bool FN) {
        DBG("mute: hold" << (FN ? " FN" : ""));
        //TODO: implement mute hold
        //TODO: implement fn mute hold
    });

    owner->commandQueue.registerCommandAction(Cmd::Mute, Type::DoublePress, TriggerType::Instant, [this] (bool FN) {
        DBG("mute: double press" << (FN ? " FN" : ""));
        //TODO: implement mute double press
        //TODO: implement fn mute double press
    });

    owner->commandQueue.registerCommandAction(Cmd::Mute, Type::DoubleHold, TriggerType::Instant, [this] (bool FN) {
        DBG("mute: double hold" << (FN ? " FN" : ""));
        //TODO: implement mute double hold
        //TODO: implement fn mute double hold
    });
}

void Commands::registerPlayCommandActions() {
    owner->commandQueue.registerCommandAction(Cmd::Play, Type::Press, TriggerType::OnUpBeat, [this] (bool FN) {
        DBG("play: press" << (FN ? " FN" : ""));
        //TODO: implement play press
        //TODO: implement fn play press
    });

    owner->commandQueue.registerCommandAction(Cmd::Play, Type::Hold, TriggerType::OnUpBeat, [this] (bool FN) {
        DBG("play: hold" << (FN ? " FN" : ""));
        //TODO: implement play hold
        //TODO: implement fn play hold
    });

    owner->commandQueue.registerCommandAction(Cmd::Play, Type::DoublePress, TriggerType::Instant, [this] (bool FN) {
        DBG("play: double press" << (FN ? " FN" : ""));
        //TODO: implement play double press
        //TODO: implement fn play double press
    });

    owner->commandQueue.registerCommandAction(Cmd::Play, Type::DoubleHold, TriggerType::Instant, [this] (bool FN) {
        DBG("play: double hold" << (FN ? " FN" : ""));
        //TODO: implement play double hold
        //TODO: implement fn play double hold
    });
}



void Commands::registerRecordCommandActions() {
    owner->commandQueue.registerCommandAction(Cmd::Record, Type::Press, TriggerType::Instant, [this] (bool FN) {
        DBG("wants to record" << (FN ? " FN" : ""));
        //TODO: implement record press
        //TODO: implement fn record press
    });
    
    owner->commandQueue.registerCommandAction(Cmd::Record, Type::Press, TriggerType::OnBeat, [this] (bool FN) {
        DBG("wants to record onBeat" << (FN ? " FN" : ""));
        //TODO: implement record press
        //TODO: implement fn record press
    });

    owner->commandQueue.registerCommandAction(Cmd::Record, Type::Press, TriggerType::OnUpBeat, [this] (bool FN) {
        DBG("wants to record onUpBeat" << (FN ? " FN" : ""));
        //TODO: implement record press
        //TODO: implement fn record press
    });

    owner->commandQueue.registerCommandAction(Cmd::Record, Type::Hold, TriggerType::Instant, [this] (bool FN) {
        DBG("record: hold" << (FN ? " FN" : ""));
        //TODO: implement record hold
        //TODO: implement fn record hold
    });

    owner->commandQueue.registerCommandAction(Cmd::Record, Type::DoublePress, TriggerType::OnBeat, [this] (bool FN) {
        DBG("cancel record" << (FN ? " FN" : ""));
        //TODO: implement record double press
        //TODO: implement fn record double press
    });

    owner->commandQueue.registerCommandAction(Cmd::Record, Type::DoubleHold, TriggerType::Instant, [this] (bool FN) {
        DBG("record: double hold" << (FN ? " FN" : ""));
        //TODO: implement record double hold
        //TODO: implement fn record double hold
    });
}



void Commands::registerTrackCommandActions() {
    int firstTrackEnum = Cmd::Track1;
    for (int track = 0;track < Config::Tracks::count; track++) {
        Cmd trackCmd = static_cast<Cmd>(track + firstTrackEnum);

        owner->commandQueue.registerCommandAction(trackCmd, Type::Press, TriggerType::Instant, [this, track] (bool FN) {
            if (!FN) owner->tracks.all(Tracks::Type::Select, Tracks::Action::Toggle);
            if (FN) owner->effects.all(Effects::Type::Select, Effects::Action::Toggle);
        });
        
        owner->commandQueue.registerCommandAction(trackCmd, Type::Hold, TriggerType::Instant, [this, track] (bool FN) {
            DBG("track"<< track <<": hold"  << (FN ? " FN" : ""));
        });
        
        owner->commandQueue.registerCommandAction(trackCmd, Type::DoublePress, TriggerType::Instant, [this, track] (bool FN) {
            DBG("track"<< track <<": double press"  << (FN ? " FN" : ""));
        });
        
        owner->commandQueue.registerCommandAction(trackCmd, Type::DoubleHold, TriggerType::Instant, [this, track] (bool FN) {
            DBG("track"<< track <<": double hold"  << (FN ? " FN" : ""));
        });

    }

}
