#pragma once
#include <JuceHeader.h>
#include "../../../Config.h"
#include "Track.h"

class Tracks {

    using Action = Track::Action;
    using ActionMode = Track::ActionMode;

public:
    Tracks() {
        for (int track = 0; track < Config::Tracks::count; track++) {
            Track* trk = new Track();
            trk->setOwner(this);
            trk->setIndex(track);
            tracks.add(trk);
        }
    }
    
    bool doForAllTracks(Action action, ActionMode mode = ActionMode::Single) {
        for (int track = 0; track < Config::Tracks::count; track++) doForTrack(track, action, mode);
        return true;
    }

    bool doForSelectedTracks(Action action, ActionMode mode = ActionMode::Single) {
        for (int track = 0; track < Config::Tracks::count; track++) doForTrack(track, action, mode, true);
        return true;
    }
    
    bool doForLastSelectedOrFreeTrack(Action action, ActionMode mode = ActionMode::Single) {
        if (!hasTracksSelected()) {
            Track* trk = getFirstFreeTrack();
            if (trk == nullptr) return false;
            
            trk->executeAction(action, mode);
        } else {
            doForTrack(lastSelectedTrackIndex, action, mode, true);
        }
        return true;
    }

    bool doForTrack(int track, Action action, ActionMode mode = ActionMode::Single, bool mustBeSelected = false) {
        Track* trk = tracks[track];
        if (!trk->isSelected() && mustBeSelected) return false;
        trk->executeAction(action, mode);
        return true;
    }
    
    int setLastSelectedTrackIndex(int index) {
        lastSelectedTrackIndex = index;
        return lastSelectedTrackIndex;
    }
    
    bool hasTracksSelected() {
        return lastSelectedTrackIndex != -1;
    }
    
    Track* getFirstFreeTrack() {
        for (int track = 0; track < Config::Tracks::count; track++) {
            Track* trk = tracks[track];
            if (!trk->hasRecords()) {
                return trk;
            }
        }
        
        return nullptr;
    }
    
    juce::OwnedArray<Track>& getTracks() {
        return tracks;
    }
 
private:
    int lastSelectedTrackIndex = -1;
    juce::OwnedArray<Track> tracks;
};
