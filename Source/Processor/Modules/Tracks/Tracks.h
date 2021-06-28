#pragma once
#include <JuceHeader.h>
#include "../../../Config.h"
#include "Track.h"

class SpoolProcessor;

class Tracks {

    using Action = Track::Action;
    using ActionMode = Track::ActionMode;

public:
    SpoolProcessor* owner;

    Tracks(SpoolProcessor* owner) : owner(owner) {
        for (int track = 0; track < Config::trackCount; track++) {
            tracks.add(new Track(this, track));
        }
    }
    
    void prepareToPlay(double sampleRate, int samplesPerBlock) {
        for (Track* track : tracks) {
            track->prepareToPlay(sampleRate, samplesPerBlock);
        }
    }
    
    void processBlockBefore(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
        for (Track* track : tracks) track->processBlockBefore(buffer, midiMessages);
    };
    
    void processBlockAfter(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
        for (Track* track : tracks) track->processBlockAfter(buffer, midiMessages);
    };
    
    void beatCallback(int beat, bool isUpBeat) {
        for (int track = 0; track < Config::trackCount; track++) {
            Track* trk = tracks[track];
            trk->beatCallback(beat, isUpBeat);
        }
    }
    
    bool doForAllTracks(Action action, ActionMode mode = ActionMode::Single) {
        for (int track = 0; track < Config::trackCount; track++) doForTrack(track, action, mode);
        return true;
    }

    bool doForSelectedTracks(Action action, ActionMode mode = ActionMode::Single) {
        for (int track = 0; track < Config::trackCount; track++) doForTrack(track, action, mode, true);
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
    
    Track* getTrack(int index) {
        return tracks[index];
    }
    
    int setOverdubLayer(int index) {
        overdubLayer = index;
        return overdubLayer;
    };
    
    int getOverdubLayer() {
        return overdubLayer;
    };
    
    void unsetOverdubLayer() {
        overdubLayer = -1;
    };
    
    bool hasOverdubLayer() {
        return overdubLayer != -1;
    };
    
    int setLastSelectedTrackIndex(int index) {
        lastSelectedTrackIndex = index;
        return lastSelectedTrackIndex;
    }
    
    bool hasTracksSelected() {
        return lastSelectedTrackIndex != -1;
    }
    
    Track* getFirstFreeTrack() {
        for (int track = 0; track < Config::trackCount; track++) {
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
    int overdubLayer = -1;
    int lastSelectedTrackIndex = -1;
    juce::OwnedArray<Track> tracks;
};