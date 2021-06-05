#pragma once

#include <JuceHeader.h>
#include "Loop.h"
#include "../../../Config.h"

class Tracks;

class Track {
public:
    enum Action {
        Select,
        Mute,
        Cue,
        Play,
        Stop,
        Record,
        CancelRecord,
        Clear
    };

    enum ActionMode {
        Single,
        On,
        Off,
        Toggle
    };
    
    void setOwner(Tracks* owner) {
        this->owner = owner;
    }
    
    int setIndex(int index) {
        trackIndex = index;
        return index;
    }
    
    void executeAction(Action action, ActionMode mode) {
        switch (action) {
            case Action::Select:
                select(mode);
                break;
            case Action::Mute:
                mute(mode);
                break;
            case Action::Cue:
                cue(mode);
                break;
            case Action::Play:
                play(mode);
                break;
            case Action::Stop:
                stop(mode);
                break;
            case Action::Record:
                record();
                break;
            case Action::CancelRecord:
                cancelRecord();
                break;
            case Action::Clear:
                clear();
                break;
            default:
                break;
        }
    }
    
    void select(ActionMode mode);
    void mute(ActionMode mode);
    void cue(ActionMode mode);
    void play(ActionMode mode);
    void stop(ActionMode mode);
    void record();
    void cancelRecord();
    void clear();
    
    
    // flags
    bool hasRecords() {
        return _hasRecords;
    }
    
    bool isSelected() {
        return _isSelected;
    }
    
private:
    Tracks* owner;

    int trackIndex = -1;
    bool _isPlaying = false;
    bool _isStopped = false;
    bool _isMuted = false;
    bool _isCueued = false;
    bool _isSelected = false;
    bool _hasRecords = false;

    juce::OwnedArray<Loop> loops;
    
    bool getValueBasedOnMode(bool value, ActionMode mode) {
        switch (mode) {
            case ActionMode::On:
                value = true;
                break;
            case ActionMode::Off:
                value = true;
                break;
            case ActionMode::Toggle:
                value = !value;
                break;
            default:
                break;
        }

        return value;
    }
    
    
    // iterate all tracks and set if any track is selected
    // if not set lastSelectedTrackIndex to -1
    void setLastSelectedTrackIndex();
};
