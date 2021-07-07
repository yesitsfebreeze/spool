#include "Track.h"

#include "Processor/Modules/Tracks/Tracks.h"
#include "Processor/SpoolProcessor.h"
#include "Processor/Modules/ControlGroup/ControlGroup.h"


Track::Track(Tracks* owner, int index, ParameterValue& volume, ParameterValue& balance) : owner(owner), index(index), volume(volume), balance(balance) {
    for (int smpl = 0; smpl < Config::TrackCount; smpl++) {
        sampleHolders.add(new SampleHolder(this, index, smpl));
    }

    effects.reset(new Effects(owner->owner, index));
    setParameterDefaults();
}

void Track::setParameterDefaults() {
    volume.set(0.75f, true);
    balance.set(0.5f, true);
}
 
void Track::prepareToPlay(double sampleRate, int samplesPerBlock) {
    effects->prepareToPlay(sampleRate, samplesPerBlock);
    for (SampleHolder* sampleHolder : sampleHolders) {
        sampleHolder->prepareToPlay(sampleRate, sampleRate);
    }
}

void Track::processBlockBefore(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
    for (SampleHolder* sampleHolder : sampleHolders) sampleHolder->processBlockBefore(buffer, midiMessages);
    if (hasRecords()) effects->processBlockBefore(buffer, midiMessages);
}


void Track::processBlockAfter(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
    int numChannels = buffer.getNumChannels();
    int numSamples = buffer.getNumSamples();
    trackBuffer.setSize(numChannels, numSamples);
    trackBuffer.clear();
    
    for (SampleHolder* sampleHolder : sampleHolders) sampleHolder->processBlockAfter(trackBuffer, midiMessages);
    
    if (hasRecords()) effects->processBlockAfter(trackBuffer, midiMessages);
    
    trackBuffer.applyGain(volume.percent);
    for (int ch = 0; ch < numChannels; ++ch) {
        buffer.addFrom(ch, 0, trackBuffer, ch, 0, numSamples);
    }
}

void Track::beatCallback(int beat, bool isUpBeat) {
    for (SampleHolder* sampleHolder : sampleHolders) {
        sampleHolder->beatCallback(beat, isUpBeat);
    }
}

void Track::select(Track::Mode mode) {
    if (owner->owner->isEffectMode()) {
        bool value = getValueBasedOnMode(_isEffectSelected, mode);
        _isEffectSelected = value;
        return;
    }
    
    
    bool value = getValueBasedOnMode(_isSelected, mode);
    _isSelected = value;
    if (value == true) owner->setLastSelectedTrackIndex(index);
    
    setLastSelectedTrackIndex();
};

void Track::mute(Track::Mode mode) {
    bool value = getValueBasedOnMode(_isMuted, mode);
    _isMuted = value;

    if (owner->hasSampleLayer()) {
        sampleHolders[owner->getSampleLayer()]->mute(_isMuted);
    } else {
        for (SampleHolder* sampleHolder : sampleHolders) sampleHolder->mute(_isMuted);
    }
};

void Track::cue(Track::Mode mode) {
    bool value = getValueBasedOnMode(_isCueued, mode);
    _isCueued = value;
    
    //TODO: implement cue channel
    if (_isCueued) {
        DBG("on cue");
    } else {
        DBG("not on cue");
    }
};

void Track::play(Track::Mode mode) {
    bool value = getValueBasedOnMode(_isPlaying, mode);
    _isPlaying = value;
    
    if (owner->hasSampleLayer()) {
        sampleHolders[owner->getSampleLayer()]->play(_isPlaying);
    } else {
        for (SampleHolder* sampleHolder : sampleHolders) sampleHolder->play(_isPlaying);
    }
};

void Track::stop(Track::Mode mode) {
    bool value = getValueBasedOnMode(_isStopped, mode);
    _isStopped = value;
    
    if (owner->hasSampleLayer()) {
        sampleHolders[owner->getSampleLayer()]->stop(_isStopped);
    } else {
        for (SampleHolder* sampleHolder : sampleHolders) sampleHolder->stop(_isStopped);
    }
};

void Track::restart() {
    for (SampleHolder* sampleHolder : sampleHolders) {
        sampleHolder->restart();
    }
}

void Track::record() {
    int recordLength = owner->owner->getRecordLength();
    
    for (SampleHolder* sampleHolder : sampleHolders) {
        if (!sampleHolder->hasSample()) {
            sampleHolder->wantsToRecord(recordLength);
            return;
        }
    }
    
    
    //TODO: Flash all buttons red to indicate full buffer
    DBG("all samples are filled");
};

void Track::cancelRecord() {
    for (SampleHolder* sampleHolder : sampleHolders) {
        sampleHolder->cancelRecord();
    }
};

void Track::clear() {
    if (owner->hasSampleLayer()) {
        sampleHolders[owner->getSampleLayer()]->clear();
    } else {
        for (SampleHolder* sampleHolder : sampleHolders) sampleHolder->clear();
    }
};

bool Track::isGrouped() {
    return (isInGroup(0) || isInGroup(1));
}

bool Track::isInGroup(int group) {
    ControlGroup controlGroup = nullptr;
    if (group == 0) controlGroup = owner->owner->controlGroupA;
    if (group == 1) controlGroup = owner->owner->controlGroupB;
    return controlGroup.containsTrack(index);
}

int Track::getGroup() {
    ControlGroup groupA = owner->owner->controlGroupA;
    ControlGroup groupB = owner->owner->controlGroupB;
    if (groupA.containsTrack(index)) return 0;
    if (groupB.containsTrack(index)) return 1;

    return -1;
}

int Track::isInEffectGroup(int group) {
    ControlGroup controlGroup = nullptr;
    if (group == 0) controlGroup = owner->owner->controlGroupA;
    if (group == 1) controlGroup = owner->owner->controlGroupB;
    return controlGroup.containsEffect(index);
}

void Track::addTrackToGroupA() {
    owner->owner->controlGroupA.addTrack(index);
}

void Track::removeTrackFromGroupA() {
    owner->owner->controlGroupA.removeTrack(index);
}

void Track::addTrackToGroupB() {
    owner->owner->controlGroupB.addTrack(index);
}

void Track::removeTrackFromGroupB() {
    owner->owner->controlGroupB.removeTrack(index);
}

void Track::removeTrackFromAllGroups() {
    owner->owner->controlGroupA.removeTrack(index);
    owner->owner->controlGroupB.removeTrack(index);
}

void Track::addEffectToGroupA() {
    owner->owner->controlGroupA.addEffect(index);
}

void Track::removeEffectFromGroupA() {
    owner->owner->controlGroupA.removeEffect(index);
}

void Track::addEffectToGroupB() {
    owner->owner->controlGroupB.addEffect(index);
}

void Track::removeEffectFromGroupB() {
    owner->owner->controlGroupB.removeEffect(index);
}


// private
void Track::setLastSelectedTrackIndex() {
   bool hasTracksSelected = false;
   juce::OwnedArray<Track>& tracks = owner->getTracks();
   for (int trk = 0; trk < Config::TrackCount; trk++) {
       if (tracks[trk]->isSelected()) {
           hasTracksSelected = true;
       }
   }

   if (!hasTracksSelected) owner->setLastSelectedTrackIndex(-1);
}


bool Track::getValueBasedOnMode(bool value, Track::Mode mode) {
    switch (mode) {
        case Track::Mode::On:
            value = true;
            break;
        case Track::Mode::Off:
            value = false;
            break;
        case Track::Mode::Toggle:
            value = !value;
            break;
        default:
            break;
    }

    return value;
}
