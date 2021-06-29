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
        for (int track = 0; track < Config::TrackCount; track++) {
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
        for (int track = 0; track < Config::TrackCount; track++) {
            Track* trk = tracks[track];
            trk->beatCallback(beat, isUpBeat);
        }
    }
    
    bool doForAllTracks(Action action, ActionMode mode = ActionMode::Single) {
        for (int track = 0; track < Config::TrackCount; track++) doForTrack(track, action, mode);
        return true;
    }

    bool doForUnselectedTracks(Action action, ActionMode mode = ActionMode::Single) {
        for (int track = 0; track < Config::TrackCount; track++) doForTrack(track, action, mode, false, true);
        return true;
    }
    
    bool doForSelectedTracks(Action action, ActionMode mode = ActionMode::Single) {
        for (int track = 0; track < Config::TrackCount; track++) doForTrack(track, action, mode, true);
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

    bool doForTrack(int track, Action action, ActionMode mode = ActionMode::Single, bool mustBeSelected = false, bool mustBeUnselected = false) {
        Track* trk = tracks[track];
        if (!trk->isSelected() && mustBeSelected) return false;
        if (trk->isSelected() && mustBeUnselected) return false;
        trk->executeAction(action, mode);
        return true;
    }
    
    Track* getTrack(int index) {
        return tracks[index];
    }
    
    int setSampleLayer(int index) {
        sampleLayer = index;
        return sampleLayer;
    };
    
    int getSampleLayer() {
        return sampleLayer;
    };
    
    void unsetSampleLayer() {
        sampleLayer = -1;
    };
    
    bool hasSampleLayer() {
        return sampleLayer != -1;
    };
    
    int setLastSelectedTrackIndex(int index) {
        lastSelectedTrackIndex = index;
        return lastSelectedTrackIndex;
    }
    
    bool hasTracksSelected() {
        return lastSelectedTrackIndex != -1;
    }
    
    Track* getFirstFreeTrack() {
        for (int track = 0; track < Config::TrackCount; track++) {
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

    void addSelectedTrack(int trackIndex) {
        selectedTracks[trackIndex] = true;
    }
    
    void removeSelectedTrack(int trackIndex) {
        selectedTracks[trackIndex] = false;
    }
    
 
private:
    int sampleLayer = -1;
    int lastSelectedTrackIndex = -1;
    juce::OwnedArray<Track> tracks;
    bool selectedTracks[];
};
