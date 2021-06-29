#pragma once
#include <JuceHeader.h>
#include "../../../Config.h"
#include "Track.h"
#include "../Effects/Effect.h"

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
        for (Track* track : tracks) track->beatCallback(beat, isUpBeat);
    }
    
    bool doForAllTracks(Action action, ActionMode mode = ActionMode::Single) {
        for (Track* track : tracks) doForTrack(track->getIndex(), action, mode);
        return true;
    }

    bool doForUnselectedTracks(Action action, ActionMode mode = ActionMode::Single) {
        for (Track* track : tracks) doForTrack(track->getIndex(), action, mode, false, true);
        return true;
    }
    
    bool doForSelectedTracks(Action action, ActionMode mode = ActionMode::Single) {
        for (Track* track : tracks) doForTrack(track->getIndex(), action, mode, true);
        return true;
    }

    bool doForLastSelectedOrFreeTrack(Action action, ActionMode mode = ActionMode::Single) {
        if (!hasTracksSelected()) {
            Track* track = getFirstFreeTrack();
            if (track == nullptr) return false;
            
            track->executeAction(action, mode);
        } else {
            doForTrack(lastSelectedTrackIndex, action, mode, true);
        }
        return true;
    }

    bool doForTrack(int trackIndex, Action action, ActionMode mode = ActionMode::Single, bool mustBeSelected = false, bool mustBeUnselected = false) {
        Track* track = tracks[trackIndex];
        if (!track->isSelected() && mustBeSelected) return false;
        if (track->isSelected() && mustBeUnselected) return false;
        track->executeAction(action, mode);
        return true;
    }
    
    void doCallbackForTracksInGroup(int groupIndex, std::function<void(Track* track)> cb) {
        for (Track* track : tracks) if (track->getGroup() == groupIndex) cb(track);
    }
    
    void doCallbackForEffectsInGroup(int groupIndex, std::function<void(Track* track, Effect* effect)> cb) {
        doCallbackForTracksInGroup(groupIndex, [this, groupIndex, cb] (Track* track) {
            for (Track* track : tracks) if (track->isInEffectGroup(groupIndex)) cb(track, track->effects->effects[groupIndex]);
        });        
    }
    
    void doCallbackForSelectedTracks(int groupIndex, std::function<void(Track* track)> cb) {
        for (Track* track : tracks) if (track->isSelected()) cb(track);
    }
    
    void doCallbackForUnselectedTracks(int groupIndex, std::function<void(Track* track)> cb) {
        for (Track* track : tracks) if (!track->isSelected()) cb(track);
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
        for (Track* track : tracks) if (!track->hasRecords()) return track;
        return nullptr;
    }
    
    juce::OwnedArray<Track>& getTracks() {
        return tracks;
    }
 
private:
    int sampleLayer = -1;
    int lastSelectedTrackIndex = -1;
    juce::OwnedArray<Track> tracks;
};
