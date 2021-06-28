#include "Track.h"
#include "Tracks.h"
#include "../../SpoolProcessor.h"


Track::Track(Tracks* owner, int trackIndex) : owner(owner), trackIndex(trackIndex) {
    for (int index = 0; index < Config::TrackCount; index++) {
        sampleHolders.add(new SampleHolder(this, trackIndex, index));
    }

    effects.reset(new Effects(owner->owner, trackIndex));
    

    Parameters::getValueTree().addListener(this);
}

void Track::valueTreePropertyChanged(juce::ValueTree& tree, const juce::Identifier& param) {
    if (!Parameters::isTrack(tree, trackIndex)) return;
    juce::String paramName = param.toString();
    
    DBG(paramName);
};

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
    
    for (int ch = 0; ch < numChannels; ++ch) {
        buffer.addFrom(ch, 0, trackBuffer, ch, 0, numSamples);
    }
}

void Track::beatCallback(int beat, bool isUpBeat) {
    for (SampleHolder* sampleHolder : sampleHolders) {
        sampleHolder->beatCallback(beat, isUpBeat);
    }
}

void Track::select(ActionMode mode) {
    bool value = getValueBasedOnMode(_isSelected, mode);
    
    _isSelected = value;
    if (value == true) {
        owner->setLastSelectedTrackIndex(trackIndex);
        owner->addSelectedTrack(trackIndex);
    } else {
        owner->removeSelectedTrack(trackIndex);
    };
    
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
    
    //TODO: implement cue channel
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
   for (int trk = 0; trk < Config::TrackCount; trk++) {
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
