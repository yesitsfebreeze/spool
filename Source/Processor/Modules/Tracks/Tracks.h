#pragma once
#include <JuceHeader.h>
#include "Config.h"

#include "Processor/Modules/Tracks/Track.h"
#include "Processor/Modules/Effects/Base/Effect.h"

class SpoolProcessor;

class Tracks {

public:
    SpoolProcessor* owner;

    Tracks(SpoolProcessor* owner) : owner(owner) {
        for (int track = 0; track < Config::TrackCount; track++) {
            ParameterValue& volume = Parameters::get((Config::TrackID) track, Config::Parameter::Volume);
            ParameterValue& balance = Parameters::get((Config::TrackID) track, Config::Parameter::Balance);
            tracks.add(new Track(this, track, volume, balance));
        }
    }
    
    void prepareToPlay(double sampleRate, int samplesPerBlock) {
        for (Track* track : tracks) {
            track->prepareToPlay(sampleRate, samplesPerBlock);
        }
    }
    
    void processBlockBefore(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
        for (Track* track : tracks) {
            track->processBlockBefore(buffer, midiMessages);
        }
    };
    
    void processBlockAfter(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
        for (Track* track : tracks) {
            track->processBlockAfter(buffer, midiMessages);
        }
    };
    
    void beatCallback(int beat, bool isUpBeat) {
        for (Track* track : tracks) {
            track->beatCallback(beat, isUpBeat);
        }
    }
    
    bool doForAllTracks(Track::Action action, Track::Mode mode = Track::Mode::Single, ControlGroup::Group group = ControlGroup::Group::Unassinged) {
        for (Track* track : tracks) {
            doForTrack(track->getIndex(), action, mode, false, false, group);
        }
        return true;
    }

    bool doForUnselectedTracks(Track::Action action, Track::Mode mode = Track::Mode::Single, ControlGroup::Group group = ControlGroup::Group::Unassinged) {
        for (Track* track : tracks) {
            doForTrack(track->getIndex(), action, mode, false, true, group);
        }
        return true;
    }
    
    bool doForSelectedTracks(Track::Action action, Track::Mode mode = Track::Mode::Single, ControlGroup::Group group = ControlGroup::Group::Unassinged) {
        for (Track* track : tracks) {
            doForTrack(track->getIndex(), action, mode, true, false, group);
        }
        return true;
    }

    bool doForLastSelectedOrFreeTrack(Track::Action action, Track::Mode mode = Track::Mode::Single) {
        if (!hasTracksSelected()) {
            Track* track = getFirstFreeTrack();
            if (track == nullptr) return false;
            
            track->executeAction(action, mode);
        } else {
            doForTrack(lastSelectedTrackIndex, action, mode, true);
        }
        return true;
    }

    bool doForTrack(int trackIndex, Track::Action action, Track::Mode mode = Track::Mode::Single, bool mustBeSelected = false, bool mustBeUnselected = false, ControlGroup::Group group = ControlGroup::Group::Unassinged) {
        Track* track = tracks[trackIndex];
        if (mustBeSelected && !track->isSelected()) return false;
        if (mustBeUnselected && track->isSelected()) return false;
        track->executeAction(action, mode, group);
        return true;
    }
    
    bool doForAllEffects(Track::Action action, Track::Mode mode = Track::Mode::Single, ControlGroup::Group group = ControlGroup::Group::Unassinged) {
        for (Track* track : tracks) {
            doForEffect(track->getIndex(), action, mode, false, false, group);
        }
        return true;
    }

    bool doForUnselectedEffects(Track::Action action, Track::Mode mode = Track::Mode::Single, ControlGroup::Group group = ControlGroup::Group::Unassinged) {
        for (Track* track : tracks) {
            doForEffect(track->getIndex(), action, mode, false, true, group);
        }
        return true;
    }
    
    bool doForSelectedEffects(Track::Action action, Track::Mode mode = Track::Mode::Single, ControlGroup::Group group = ControlGroup::Group::Unassinged) {
        for (Track* track : tracks) {
            doForEffect(track->getIndex(), action, mode, true, false, group);
        }
        return true;
    }
    
    bool doForEffect(int trackIndex, Track::Action action, Track::Mode mode = Track::Mode::Single, bool mustBeSelected = false, bool mustBeUnselected = false, ControlGroup::Group group = ControlGroup::Group::Unassinged) {
        Track* track = tracks[trackIndex];
        if (mustBeSelected && !track->isEffectSelected()) return false;
        if (mustBeUnselected && track->isEffectSelected()) return false;
        track->executeAction(action, mode, group);
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
        for (Track* track : tracks) {
            if (!track->hasSamples()) {
                return track;
            }
        }
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
