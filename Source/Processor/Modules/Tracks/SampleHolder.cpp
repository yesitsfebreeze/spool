#include "SampleHolder.h"
#include "Track.h"
#include "Tracks.h"


SampleHolder::SampleHolder(Track* owner, int trackIndex, int index) : owner(owner), trackIndex(trackIndex), index(index) {
    effects.reset(new Effects(owner->owner->owner, trackIndex, index));
}

SampleHolder::~SampleHolder() {
    effects.reset();
}


void SampleHolder::prepareToPlay(double sampleRate, int samplesPerBlock) {
    effects->prepareToPlay(sampleRate, samplesPerBlock);
}

void SampleHolder::processBlockBefore(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
    record(buffer);
    if (_hasSample) effects->processBlockBefore(buffer, midiMessages);
};

void SampleHolder::processBlockAfter(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
    if (_hasSample) effects->processBlockAfter(buffer, midiMessages);
    playBuffer(buffer);
};


void SampleHolder::beatCallback(int beat, bool isUpBeat) {
    setRecordState(beat, isUpBeat);
    setPlayState(beat, isUpBeat);
}

void SampleHolder::wantsToRecord(int beatLength) {
    this->beatLength = beatLength;
    _wantsToRecord = true;
    owner->setWantsToRecord(_wantsToRecord);
}

void SampleHolder::cancelRecord() {
    if (!_wantsToRecord) return;

    this->beatLength = 0;
    _wantsToRecord = false;
    owner->setWantsToRecord(_wantsToRecord);
}

void SampleHolder::startRecording(int beat) {
    recordedBeats = 0;
    _isRecording = true;
    owner->setRecording(_isRecording);
    _wantsToRecord = false;
    owner->setWantsToRecord(_wantsToRecord);
}

void SampleHolder::stopRecording(int beat) {
    _isRecording = false;
    owner->setRecording(_isRecording);
    _wantsToRecord = false;
    owner->setWantsToRecord(_wantsToRecord);
    _hasSample = true;
    owner->setHasRecords(true);
    startBeat = beat;
    
    if (!_isPlaying) {
        play(true);
    }
    
    //TODO: play with currentBeatOffset after track is a new recording
}


void SampleHolder::setRecordState(int beat, bool isUpBeat) {
    if (isUpBeat && _wantsToRecord) {
        startRecording(beat);
        return;
    }
    
    if (!_isRecording) return;
    if (recordedBeats + 1 == beatLength) {
        stopRecording(beat);
        return;
    }
    
    recordedBeats++;
}


void SampleHolder::setPlayState(int beat, bool isUpBeat) {
    if (sampleSize == 0 || _isRecording) return;
    if (beatsPlayed % beatLength == 0) restart();
    
    beatsPlayed++;
}

void SampleHolder::restart() {
    samplesPlayed = 0;
    beatsPlayed = 0;
}

void SampleHolder::clear() {
    restart();
    sampleBuffer.clear(0, 0);
    sampleSize = 0;
    _hasSample = false;
    
    // always reset has Recordings of track after deletion
    owner->setHasRecords(false);
    for (SampleHolder* sampleHolder : owner->sampleHolders) {
        if (sampleHolder->hasSample()) owner->setHasRecords(true);
    }
}

void SampleHolder::record(juce::AudioBuffer<float>& buffer) {
    if (!_isRecording) return;
    
    int numSamples = buffer.getNumSamples();
    int numInputChannels = buffer.getNumChannels();
    sampleBuffer.setSize(numInputChannels, sampleSize + numSamples, true);
    sampleBuffer.copyFrom(0, sampleSize, buffer, 0, 0, numSamples);
    sampleSize += numSamples;
}


void SampleHolder::addToBuffer(juce::AudioBuffer<float>& buffer, int sourceChannel, int outNumSamples, int numSamples) {
    if (_isMuted) return;

    int numRemainingSamples = numSamples - samplesPlayed;
    int numLeftoverSamples = outNumSamples - numRemainingSamples;

    if (outNumSamples > numRemainingSamples) {
        for (int ch = 0; ch < buffer.getNumChannels(); ++ch) {
            // add remaining samples to the buffer
            buffer.addFrom(ch, 0, sampleBuffer, sourceChannel, samplesPlayed, numRemainingSamples);
            // add leftover samples from start of the buffer
            buffer.addFrom(ch, numRemainingSamples, sampleBuffer, sourceChannel, 0, numLeftoverSamples);
        }

    } else {
        for (int ch = 0; ch < buffer.getNumChannels(); ++ch) {
            buffer.addFrom(ch, 0, sampleBuffer, sourceChannel, samplesPlayed, outNumSamples);
        }
    }
}


void SampleHolder::playBuffer(juce::AudioBuffer<float>& buffer) {
    if (_isRecording) return;
    if (sampleSize == 0) return;
    if (!_isPlaying) return;
    
    int sourceChannel = 0;

    int outNumSamples = buffer.getNumSamples();
    int numSamples = sampleBuffer.getNumSamples();
    
    addToBuffer(buffer, sourceChannel, outNumSamples, numSamples);

    samplesPlayed += outNumSamples;
    samplesPlayed = samplesPlayed % numSamples;
}




