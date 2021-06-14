#pragma once

#include <JuceHeader.h>

namespace Config {

    const float updateHz = 60; // hertz
    const double defaultPBM = 128;
    const int defaultRecordLength = 4; // bars
    const int MaxRecordLength = 32;
    const int trackCount = 16;

    // timing relevant stuff
    const int doublePressTimeWindow = 250;
    const int holdTimeout = 333; // ms
    const int debounceTimeout = 200; // ms
    const int FNResetDelay = 100; // ms
    const int TrackResetDelay = 150; // ms
    const int recordLengthDebounceTime = 1000; // ms

    namespace Command {
        enum ID {
            Settings = 1,
            Function,
            Mute,
            Play,
            Record,
            Track1,
            Track2,
            Track3,
            Track4,
            Track5,
            Track6,
            Track7,
            Track8,
            Track9,
            Track10,
            Track11,
            Track12,
            Track13,
            Track14,
            Track15,
            Track16
        };
    }

    namespace Parameters {
        enum SpoolParam {
            Bypass
        };
    
        enum TrackParam {
            Volume,
            Panning
        };

        enum EffectParam {
            Wet,
            ValueOne,
            ValueTwo,
        };
    }
}


