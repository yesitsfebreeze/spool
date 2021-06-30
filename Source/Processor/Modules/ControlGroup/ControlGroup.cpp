#include "ControlGroup.h"
#include "../Tracks/Track.h"
#include "../Effects/Effect.h"
#include "../../SpoolProcessor.h"

void ControlGroup::doForTracks(std::function<void(Track* track)> callback) {
    for (int i = 0; i < Config::TrackCount; i++) {
        if (tracks.at(i)) callback(processor->tracks->getTrack(i));
    }
}

void ControlGroup::doForEffects(std::function<void(Track* track, Effect* effect)> callback) {
    doForTracks([this, callback] (Track* track){
        for (int i = 0; i < Config::TrackCount; i++) {
            if (effects.at(i)) callback(track, track->effects->getEffect(i));
        }
    });
}


bool ControlGroup::containsTrack(int index) {
    return tracks.at(index);
}

bool ControlGroup::containsEffect(int index) {
    return effects.at(index);
}
