#include "Dispatcher.h"

#include "Processor/SpoolProcessor.h"

Dispatcher::Dispatcher(SpoolProcessor* owner) : owner(owner) {
    
}

bool Dispatcher::single(int index, Action action, Mode mode, Group group, SelectionRequirement selection) {
    Track* track = owner->tracks->getTrack(index);
    
    if (owner->isEffectMode()) {
        if (selection == SelectionRequirement::Selected && !track->isEffectSelected()) return false;
        if (selection == SelectionRequirement::Unselected && track->isEffectSelected()) return false;
    } else {
        if (selection == SelectionRequirement::Selected && !track->isSelected()) return false;
        if (selection == SelectionRequirement::Unselected && track->isSelected()) return false;
    }

    track->executeAction(action, mode, group);
    return true;
};

bool Dispatcher::all(Action action, Mode mode, Group group) {
    for (Track* track : owner->tracks->getTracks()) {
        single(track->getIndex(), action, mode, group);
    }
    return true;
};

bool Dispatcher::selected(Action action, Mode mode, Group group) {
    for (Track* track : owner->tracks->getTracks()) {
        single(track->getIndex(), action, mode, group, SelectionRequirement::Selected);
    }
    return true;
};

bool Dispatcher::unselected(Action action, Mode mode, Group group) {
    for (Track* track : owner->tracks->getTracks()) {
        single(track->getIndex(), action, mode, group, SelectionRequirement::Unselected);
    }
    return true;
};

bool Dispatcher::freeOrLastSelected(Action action, Mode mode) {
    if (!owner->tracks->hasTracksSelected()) {
        Track* track = owner->tracks->getFirstFreeTrack();
        if (track == nullptr) return false;
        
        single(track->getIndex(), action, mode, Group::Unassinged, SelectionRequirement::None);
        return  true;
    }
    
    single(owner->tracks->getLastSelectedTrackIndex(), action, mode, Group::Unassinged, SelectionRequirement::Selected);
    return true;
};
