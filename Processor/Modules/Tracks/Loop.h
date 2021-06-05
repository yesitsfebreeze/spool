#pragma once

class Track;

class Loop {
public:
    Loop(Track* owner, int trackIndex, int loopIndex) : owner(owner), trackIndex(trackIndex), loopIndex(loopIndex) {
        
    }
    
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
//  DBG(trackIndex << " : " << loopIndex);
    // only if playing && has recoring
        // play loop
        // procces loop fx
    };
        
    void record() {
        
    }
    
    void clear() {
        
    }

private:
    Track* owner;
    int trackIndex = -1;
    int loopIndex = -1;
};
