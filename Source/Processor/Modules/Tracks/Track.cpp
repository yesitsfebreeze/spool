#include "Track.h"

#include "Processor/Modules/Tracks/Tracks.h"
#include "Processor/SpoolProcessor.h"
#include "Processor/Modules/ControlGroup/ControlGroup.h"


Track::Track(Tracks* owner, int index, ParameterValue& volume, ParameterValue& balance) : owner(owner), index(index), volume(volume), balance(balance) {
    for (int smpl = 0; smpl < Config::TrackCount; smpl++) {
        samples.add(new Sample(this, index, smpl));
    }

    effects.reset(new Effects(owner->owner, index));
    setupParameters();
}

void Track::setupParameters() {
    volume.set(0.75f, true);
    balance.set(0.5f, true);
}
 
void Track::prepareToPlay(double sampleRate, int samplesPerBlock) {
    effects->prepareToPlay(sampleRate, samplesPerBlock);
    for (Sample* sample : samples) {
        sample->prepareToPlay(sampleRate, sampleRate);
    }
}

void Track::processBlockBefore(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
    for (Sample* sample : samples) sample->processBlockBefore(buffer, midiMessages);
    if (hasSamples()) effects->processBlockBefore(buffer, midiMessages);
}


void Track::processBlockAfter(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
    int numSamples = buffer.getNumSamples();
    trackBuffer.setSize(Config::CHCount, numSamples);
    trackBuffer.clear();

    if (!hasSamples()) return;

    for (Sample* sample : samples) sample->processBlockAfter(trackBuffer, midiMessages);
    effects->processBlockAfter(trackBuffer, midiMessages);

    basicProcessing.setBuffer(trackBuffer).setBalance(balance).setVolume(volume).addTo(buffer).process();
}

void Track::beatCallback(int beat, bool isUpBeat) {
    for (Sample* sample : samples) {
        sample->beatCallback(beat, isUpBeat);
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
        samples[owner->getSampleLayer()]->mute(_isMuted);
    } else {
        for (Sample* sample : samples) sample->mute(_isMuted);
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
        samples[owner->getSampleLayer()]->play(_isPlaying);
    } else {
        for (Sample* sample : samples) sample->play(_isPlaying);
    }
};

void Track::stop(Track::Mode mode) {
    bool value = getValueBasedOnMode(_isStopped, mode);
    _isStopped = value;
    
    if (owner->hasSampleLayer()) {
        samples[owner->getSampleLayer()]->stop(_isStopped);
    } else {
        for (Sample* sample : samples) sample->stop(_isStopped);
    }
};

void Track::restart() {
    for (Sample* sample : samples) {
        sample->restart();
    }
}

void Track::record() {
    int recordLength = owner->owner->getRecordLength();
    
    for (Sample* sample : samples) {
        if (!sample->isFilled()) {
            sample->wantsToRecord(recordLength);
            return;
        }
    }
    
    
    //TODO: Flash all buttons red to indicate full buffer
    DBG("all samples are filled");
};

void Track::cancelRecord() {
    for (Sample* sample : samples) {
        sample->cancelRecord();
    }
};

void Track::clear() {
    if (owner->hasSampleLayer()) {
        samples[owner->getSampleLayer()]->clear();
    } else {
        for (Sample* sample : samples) sample->clear();
    }
};

bool Track::isGrouped() {
    return (isInGroup(ControlGroup::Group::A) || isInGroup(ControlGroup::Group::B));
}

bool Track::isInGroup(ControlGroup::Group group) {
    ControlGroup* controlGroup = owner->owner->controlGroups[group];
    return controlGroup->containsTrack(index);
}

ControlGroup::Group Track::getGroup() {
    ControlGroup* controlGroupA = owner->owner->controlGroups[ControlGroup::Group::A];
    ControlGroup* controlGroupB = owner->owner->controlGroups[ControlGroup::Group::B];
    if (controlGroupA->containsTrack(index)) return ControlGroup::Group::A;
    if (controlGroupB->containsTrack(index)) return ControlGroup::Group::B;

    return ControlGroup::Group::Unassinged;
}

bool Track::isEffectGrouped() {
    return (isInEffectGroup(ControlGroup::Group::A) || isInEffectGroup(ControlGroup::Group::B));
}

bool Track::isInEffectGroup(ControlGroup::Group group) {
    ControlGroup* controlGroup = owner->owner->controlGroups[group];
    return controlGroup->containsEffect(index);
}

ControlGroup::Group Track::getEffectGroup() {
    ControlGroup* controlGroupA = owner->owner->controlGroups[ControlGroup::Group::A];
    ControlGroup* controlGroupB = owner->owner->controlGroups[ControlGroup::Group::B];
    if (controlGroupA->containsEffect(index)) return ControlGroup::Group::A;
    if (controlGroupB->containsEffect(index)) return ControlGroup::Group::B;

    return ControlGroup::Group::Unassinged;
}

void Track::setGroup(Track::Mode mode, ControlGroup::Group group) {
    bool value = getValueBasedOnMode(isInGroup(group), mode);
    if (!value) {
        owner->owner->controlGroups[group]->removeTrack(index);
    } else {
        owner->owner->controlGroups[group]->addTrack(index);
    }
}


void Track::unsetGroup() {
    owner->owner->controlGroups[ControlGroup::Group::A]->removeTrack(index);
    owner->owner->controlGroups[ControlGroup::Group::B]->removeTrack(index);
}


void Track::setEffectGroup(Track::Mode mode, ControlGroup::Group group) {
    bool value = getValueBasedOnMode(isInEffectGroup(group), mode);
    if (!value) {
        owner->owner->controlGroups[group]->removeEffect(index);
    } else {
        owner->owner->controlGroups[group]->addEffect(index);
    }
}

void Track::unsetEffectGroup() {
    owner->owner->controlGroups[ControlGroup::Group::A]->removeEffect(index);
    owner->owner->controlGroups[ControlGroup::Group::B]->removeEffect(index);
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
