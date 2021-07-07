#pragma once
#include <JuceHeader.h>
#include "Processor/Modules/Effects/Base/Effects.h"

class Track;

class Sample {
public:
    std::unique_ptr<Effects> effects;
    
    Sample(Track* owner, int trackIndex, int index);
    ~Sample();
    
    void prepareToPlay(double sampleRate, int samplesPerBlock);
    void processBlockBefore(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);
    void processBlockAfter(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);
    void beatCallback(int beat, bool isUpBeat);
    void wantsToRecord(int beatCount);
    void cancelRecord();
    void clear();
    void restart();
    
    bool isFilled() {
        return _isFilled;
    };
    
    void play(bool state) {
        _isPlaying = state;
        _isStopped = false;
    }
    
    bool isPlaying() {
        return _isPlaying;
    }
    
    void stop(bool state) {
        _isStopped = state;
        if (_isStopped) {
            restart();
            play(false);
        } else {
            play(true);
        };
    }
    
    bool isStopped() {
        return _isStopped;
    }
    
    void mute(bool state) {
        _isMuted = state;
    }
    
    bool isMuted() {
        return _isMuted;
    }


private:
    juce::AudioBuffer<float> sampleBuffer;
    Track* owner;
    int trackIndex = -1;
    int index = -1;
    bool _wantsToRecord = false;
    bool _isPlaying = false;
    bool _isStopped = true;
    bool _isRecording = false;
    bool _isMuted = false;
    bool _isFilled = false;

    int beatLength = 0;
    int sampleSize = 0;
    int recordedBeats = 0;
    int samplesPlayed = 0;
    int startBeat = 0;
    int beatsPlayed = 0;
    
    void record(juce::AudioBuffer<float>& buffer);
    void playBuffer(juce::AudioBuffer<float>& buffer);
    void setRecordState(int beat, bool isUpBeat);
    void startRecording(int beat);
    void stopRecording(int beat);
    void setPlayState(int beat, bool isUpBeat);
    
    void addToBuffer(juce::AudioBuffer<float>& buffer, int sourceChannel, int outNumSamples, int numSamples);
};
