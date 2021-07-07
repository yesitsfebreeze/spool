#include "Sample.h"

#include "Processor/Modules/Tracks/Track.h"
#include "Processor/Modules/Tracks/Tracks.h"


Sample::Sample(Track* owner, int trackIndex, int index) : owner(owner), trackIndex(trackIndex), index(index) {
    effects.reset(new Effects(owner->owner->owner, trackIndex, index));
}

Sample::~Sample() {
    effects.reset();
}


void Sample::prepareToPlay(double sampleRate, int samplesPerBlock) {
    effects->prepareToPlay(sampleRate, samplesPerBlock);
}

void Sample::processBlockBefore(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
    record(buffer);
    if (_isFilled) effects->processBlockBefore(buffer, midiMessages);
};

void Sample::processBlockAfter(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
    if (_isFilled) effects->processBlockAfter(buffer, midiMessages);
    playBuffer(buffer);
};


void Sample::beatCallback(int beat, bool isUpBeat) {
    setRecordState(beat, isUpBeat);
    setPlayState(beat, isUpBeat);
}

void Sample::wantsToRecord(int beatLength) {
    this->beatLength = beatLength;
    _wantsToRecord = true;
    owner->setWantsToRecord(_wantsToRecord);
}

void Sample::cancelRecord() {
    if (!_wantsToRecord) return;

    this->beatLength = 0;
    _wantsToRecord = false;
    owner->setWantsToRecord(_wantsToRecord);
}

void Sample::startRecording(int beat) {
    recordedBeats = 0;
    _isRecording = true;
    owner->setRecording(_isRecording);
    _wantsToRecord = false;
    owner->setWantsToRecord(_wantsToRecord);
}

void Sample::stopRecording(int beat) {
    _isRecording = false;
    owner->setRecording(_isRecording);
    _wantsToRecord = false;
    owner->setWantsToRecord(_wantsToRecord);
    _isFilled = true;
    owner->setHasSamples(true);
    startBeat = beat;
    
    if (!_isPlaying) {
        play(true);
    }
    
    //TODO: play with currentBeatOffset after track is a new recording
}


void Sample::setRecordState(int beat, bool isUpBeat) {
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


void Sample::setPlayState(int beat, bool isUpBeat) {
    if (sampleSize == 0 || _isRecording) return;
    if (beatsPlayed % beatLength == 0) restart();
    
    beatsPlayed++;
}

void Sample::restart() {
    samplesPlayed = 0;
    beatsPlayed = 0;
}

void Sample::clear() {
    restart();
    sampleBuffer.clear(0, 0);
    sampleSize = 0;
    _isFilled = false;
    
    // always reset has Recordings of track after deletion
    owner->setHasSamples(false);
    for (Sample* sample : owner->samples) {
        if (sample->isFilled()) owner->setHasSamples(true);
    }
}

void Sample::record(juce::AudioBuffer<float>& buffer) {
    if (!_isRecording) return;
    
    int numSamples = buffer.getNumSamples();
    int numInputChannels = buffer.getNumChannels();
    sampleBuffer.setSize(numInputChannels, sampleSize + numSamples, true);
    sampleBuffer.copyFrom(0, sampleSize, buffer, 0, 0, numSamples);
    sampleSize += numSamples;
}


void Sample::addToBuffer(juce::AudioBuffer<float>& buffer, int sourceChannel, int outNumSamples, int numSamples) {
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


void Sample::playBuffer(juce::AudioBuffer<float>& buffer) {
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




