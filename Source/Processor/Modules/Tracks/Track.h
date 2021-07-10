#pragma once

#include <JuceHeader.h>
#include "Config.h"

#include "Processor/Modules/Sample/Sample.h"
#include "Processor/Modules/Effects/Base/Effects.h"
#include "Processor/Modules/ControlGroup/ControlGroup.h"
#include "Processor/Modules/Parameters/ParameterValue.h"
#include "Processor/Modules/BasicProcessing/BasicProcessing.h"

class Tracks;

class Track : public juce::ValueTree::Listener {
public:
    Tracks* owner;
    std::unique_ptr<Effects> effects;
    juce::OwnedArray<Sample> samples;

    enum Action {
        Select,
        SelectEffect,
        Mute,
        Cue,
        Play,
        Stop,
        Restart,
        Record,
        CancelRecord,
        Clear,
        SetGroup,
        UnsetGroup,
        SetEffectGroup,
        UnsetEffectGroup,
    };

    enum Mode {
        Single,
        On,
        Off,
        Toggle
    };
    
    Track(Tracks* owner, int index, ParameterValue& volume, ParameterValue& balance);
    
    ~Track() {
        effects.reset();
    }
    
    void setOwner(Tracks* owner) {
        this->owner = owner;
    }
    
    int getIndex() {
        return index;
    }
    
    Config::TrackID getEnum() {
        return (Config::TrackID) index;
    }
    

    void prepareToPlay(double sampleRate, int samplesPerBlock);
    void processBlockBefore(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);
    void processBlockAfter(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);

    void beatCallback(int beat, bool isUpBeat);

    void executeAction(Track::Action action, Track::Mode mode, ControlGroup::Group group = ControlGroup::Group::Unassinged) {
        switch (action) {
            case Track::Action::Select:
                select(mode);
                break;
            case Track::Action::SelectEffect:
                select(mode);
                break;
            case Track::Action::Mute:
                mute(mode);
                break;
            case Track::Action::Cue:
                cue(mode);
                break;
            case Track::Action::Play:
                play(mode);
                break;
            case Track::Action::Stop:
                stop(mode);
                break;
            case Track::Action::Restart:
                restart();
                break;
            case Track::Action::Record:
                record();
                break;
            case Track::Action::CancelRecord:
                cancelRecord();
                break;
            case Track::Action::Clear:
                clear();
                break;
            case Track::Action::SetGroup:
                setGroup(mode, group);
                break;
            case Track::Action::UnsetGroup:
                unsetGroup();
                break;
            case Track::Action::SetEffectGroup:
                setEffectGroup(mode, group);
                break;
            case Track::Action::UnsetEffectGroup:
                unsetEffectGroup();
                break;
            default:
                break;
        }
    }
    
    void select(Track::Mode mode);
    void mute(Track::Mode mode);
    void cue(Track::Mode mode);
    void play(Track::Mode mode);
    void stop(Track::Mode mode);
    void restart();
    void record();
    void cancelRecord();
    void clear();
    void setGroup(Track::Mode mode, ControlGroup::Group group);
    void unsetGroup();
    void setEffectGroup(Track::Mode mode, ControlGroup::Group group);
    void unsetEffectGroup();
    
    //  grouping
    bool isGrouped();
    bool isInGroup(ControlGroup::Group group);
    ControlGroup::Group getGroup();
    
    bool isEffectGrouped();
    bool isInEffectGroup(ControlGroup::Group group);
    ControlGroup::Group getEffectGroup();
    
    // flags
    bool isSelected() {
        return _isSelected;
    }
    
    bool isEffectSelected() {
        return _isEffectSelected;
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
    
    bool setHasSamples(bool state) {
        _hasSamples = state;
        return _hasSamples;
    }
    
    bool hasSamples() {
        return _hasSamples;
    }
    
private:
    juce::AudioBuffer<float> trackBuffer;
    
    int index = -1;
    ParameterValue& volume;
    ParameterValue& balance;

    BasicProcessing basicProcessing;

    bool _isPressed = false;
    bool _isPlaying = false;
    bool _isStopped = true;
    bool _isMuted = false;
    bool _isCueued = false;
    bool _isSelected = false;
    bool _isEffectSelected = false;
    bool _wantsToRecord = false;
    bool _isRecording = false;
    bool _hasSamples = false;

    void setupParameters();
    void calculateBalance();
    bool getValueBasedOnMode(bool value, Track::Mode mode);
    void setLastSelectedTrackIndex();
    
    float clampValue(float value, float min, float max) {
        if (value >= max) return max;
        if (value <= min) return min;
        return value;
    }
};
