#include "Track.h"
#include "Tracks.h"
#include "../../SpoolProcessor.h"


void Track::processBlockBefore(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
    for (SampleHolder* sampleHolder : sampleHolders) sampleHolder->processBlockBefore(buffer, midiMessages);
}


void Track::processBlockAfter(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
    for (SampleHolder* sampleHolder : sampleHolders) sampleHolder->processBlockAfter(buffer, midiMessages);
    
    //TODO: procces track effects
}

void Track::beatCallback(bool isUpBeat) {
    for (SampleHolder* sampleHolder : sampleHolders) {
        sampleHolder->beatCallback(isUpBeat);
    }
}

void Track::select(ActionMode mode) {
    bool value = getValueBasedOnMode(_isSelected, mode);
    
    _isSelected = value;
    if (value == true) owner->setLastSelectedTrackIndex(trackIndex);
    
    setLastSelectedTrackIndex();
};

void Track::mute(ActionMode mode) {
    bool value = getValueBasedOnMode(_isMuted, mode);
    _isMuted = value;

    if (owner->hasOverdubLayer()) {
        sampleHolders[owner->getOverdubLayer()]->mute(_isMuted);
    } else {
        for (SampleHolder* sampleHolder : sampleHolders) sampleHolder->mute(_isMuted);
    }
};

void Track::cue(ActionMode mode) {
    bool value = getValueBasedOnMode(_isCueued, mode);
    _isCueued = value;
    if (_isCueued) {
        DBG("on cue");
    } else {
        DBG("not on cue");
    }
};

void Track::play(ActionMode mode) {
    bool value = getValueBasedOnMode(_isPlaying, mode);
    _isPlaying = value;
    
    if (owner->hasOverdubLayer()) {
        sampleHolders[owner->getOverdubLayer()]->play(_isPlaying);
    } else {
        for (SampleHolder* sampleHolder : sampleHolders) sampleHolder->play(_isPlaying);
    }
};

void Track::stop(ActionMode mode) {
    bool value = getValueBasedOnMode(_isStopped, mode);
    _isStopped = value;
    
    if (owner->hasOverdubLayer()) {
        sampleHolders[owner->getOverdubLayer()]->stop(_isStopped);
    } else {
        for (SampleHolder* sampleHolder : sampleHolders) sampleHolder->stop(_isStopped);
    }
    
    if (_isStopped) {
        DBG("stopped");
    } else {
        DBG("started");
    }
};

void Track::restart() {
    DBG(trackIndex << " restart");
}

void Track::record() {
    int recordLength = owner->owner->getRecordLength();
    
    for (SampleHolder* sampleHolder : sampleHolders) {
        if (!sampleHolder->hasSample()) {
            sampleHolder->wantsToRecord(recordLength);
            return;
        }
    }
    
    DBG("all samples are filled");
};

void Track::cancelRecord() {
    DBG(trackIndex << " cancel record");
};

void Track::clear() {
    if (owner->hasOverdubLayer()) {
        sampleHolders[owner->getOverdubLayer()]->clear();
    } else {
        for (SampleHolder* sampleHolder : sampleHolders) sampleHolder->clear();
    }
};


// private
void Track::setLastSelectedTrackIndex() {
   bool hasTracksSelected = false;
   juce::OwnedArray<Track>& tracks = owner->getTracks();
   for (int trk = 0; trk < Config::Tracks::count; trk++) {
       if (tracks[trk]->isSelected()) {
           hasTracksSelected = true;
       }
   }

   if (!hasTracksSelected) owner->setLastSelectedTrackIndex(-1);
}


bool Track::getValueBasedOnMode(bool value, ActionMode mode) {
    switch (mode) {
        case ActionMode::On:
            value = true;
            break;
        case ActionMode::Off:
            value = false;
            break;
        case ActionMode::Toggle:
            value = !value;
            break;
        default:
            break;
    }

    return value;
}
