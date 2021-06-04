#pragma once
#include <JuceHeader.h>

class Effects {
public:
    enum Action {
        On,
        Off,
        Toggle,
        Invert
    };
    
    enum Type {
        Select,
        Mute,
        Play
    };
    
    bool all(Type type, Action action) {
        DBG("invertSelection");
        return true;
    }

    bool selection(Type type, Action action) {
        DBG("deselectall");
        return true;
    }
};
