#pragma once
#include <JuceHeader.h>
#include "Track.h"


class Tracks {
public:
    Tracks() {
        
    }
    
    enum Action {
        On,
        Off,
        Toggle,
        Invert
    };
    
    enum Type {
        Select,
        Mute,
        Play
    };
    
    bool all(Type type, Action action) {
        DBG((int)type);
        DBG((int)action);
        return true;
    }

    bool selection(Type type, Action action) {
        DBG((int)type);
        DBG((int)action);
        return true;
    }
 
private:
    juce::OwnedArray<Track> tracks;
};
