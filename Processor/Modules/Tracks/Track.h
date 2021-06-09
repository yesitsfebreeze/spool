#pragma once

#include <JuceHeader.h>
#include "SampleHolder.h"
#include "../../../Config.h"
#include "../Effects/Effects.h"

class Tracks;

class Track {
public:
    Tracks* owner;
    std::unique_ptr<Effects> effects;
    juce::OwnedArray<SampleHolder> sampleHolders;

    enum Action {
        Select,
        Mute,
        Cue,
        Play,
        Stop,
        Restart,
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
    
    Track(Tracks* owner, int trackIndex) : owner(owner), trackIndex(trackIndex) {
        for (int index = 0; index < Config::trackCount; index++) {
            sampleHolders.add(new SampleHolder(this, trackIndex, index));
        }

        effects.reset(new Effects());
    }
    
    ~Track() {
        effects.reset();
    }
    
    void setOwner(Tracks* owner) {
        this->owner = owner;
    }
    
    void processBlockBefore(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);
    void processBlockAfter(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);

    void beatCallback(int beat,bool isUpBeat);

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
            case Action::Restart:
                restart();
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
    void restart();
    void record();
    void cancelRecord();
    void clear();
    
    
    // flags
    bool isSelected() {
        return _isSelected;
    }

    bool setPressed(bool state) {
        _isPressed = state;
        return _isPressed;
    }

    bool isPressed() {
        return _isPressed;
    }
    
    bool setMuted(bool state) {
        _isMuted = state;
        return _isMuted;
    }
    
    bool isMuted() {
        return _isMuted;
    }
    
    bool setRecording(bool state) {
        _isRecording = state;
        return _isRecording;
    }

    bool isRecording() {
        return _isRecording;
    }
    
    bool setWantsToRecord(bool state) {
        _wantsToRecord = state;
        return _wantsToRecord;
    }

    bool wantsToRecord() {
        return _wantsToRecord;
    }
    
    bool setHasRecords(bool state) {
        _hasRecords = state;
        return _hasRecords;
    }
    
    bool hasRecords() {
        return _hasRecords;
    }
    

    
private:
    int trackIndex = -1;
    bool _isPressed = false;
    bool _isPlaying = false;
    bool _isStopped = true;
    bool _isMuted = false;
    bool _isCueued = false;
    bool _isSelected = false;
    bool _wantsToRecord = false;
    bool _isRecording = false;
    bool _hasRecords = false;

    
    bool getValueBasedOnMode(bool value, ActionMode mode);
    void setLastSelectedTrackIndex();
};
