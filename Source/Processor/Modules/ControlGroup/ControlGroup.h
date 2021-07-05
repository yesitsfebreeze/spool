#pragma once

#include <JuceHeader.h>

#include "Config.h"

class SpoolProcessor;
class Track;
class Effect;

class ControlGroup {
public:

    enum Group {
        Unassinged,
        A,
        B
    };
    
    ControlGroup(SpoolProcessor* processor) : processor(processor) {
        for (int i = 0; i < Config::TrackCount; i++) {
            tracks.push_back(false);
            effects.push_back(false);
        }
    }
    
    void addTrack(int index) {
        tracks[index] = true;
    }
    
    void removeTrack(int index) {
        tracks[index] = false;
    }
    
    void addEffect(int index) {
        effects[index] = true;
    }
    
    void removeEffect(int index) {
        effects[index] = false;
    }

    void doForTracks(std::function<void(Track* track)> callback);
    void doForEffects(std::function<void(Track* track, Effect* effect)> callback);
    
    bool containsTrack(int index);
    bool containsEffect(int index);

    
private:
    SpoolProcessor* processor;
    std::vector<bool> tracks;
    std::vector<bool> effects;
};
