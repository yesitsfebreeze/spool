#include "Track.h"
#include "Tracks.h"



void Track::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
    for (Loop* loop : loops) {
        loop->processBlock(buffer,midiMessages);
    }
    
    // procces track effects
}
    
void Track::select(ActionMode mode){
    bool value = getValueBasedOnMode(_isSelected, mode);
    
    _isSelected = value;
    if (value == true) owner->setLastSelectedTrackIndex(trackIndex);
    
    setLastSelectedTrackIndex();
};

void Track::mute(ActionMode mode){
    bool value = getValueBasedOnMode(_isMuted, mode);
    _isMuted = value;
    if (_isMuted) {
        DBG("muted");
    } else {
        DBG("unmuted");
    }
};

void Track::cue(ActionMode mode){
    bool value = getValueBasedOnMode(_isCueued, mode);
    _isCueued = value;
    if (_isCueued) {
        DBG("on cue");
    } else {
        DBG("not on cue");
    }
};

void Track::play(ActionMode mode){
    bool value = getValueBasedOnMode(_isPlaying, mode);
    _isPlaying = value;
    if (_isPlaying) {
        DBG("playing");
    } else {
        DBG("paused");
    }
};

void Track::stop(ActionMode mode){
    bool value = getValueBasedOnMode(_isStopped, mode);
    _isStopped = value;
    
    if (_isStopped) {
        DBG("stopped");
    } else {
        DBG("started");
    }
};

void Track::record(){
    DBG(trackIndex << " record");
};

void Track::cancelRecord(){
    DBG(trackIndex << " cancel record");
};

void Track::clear(){

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
