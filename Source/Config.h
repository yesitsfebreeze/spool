#pragma once

#include <JuceHeader.h>

namespace Config {

    const float UpdateHz = 60; // hertz
    const double DefaultPBM = 128;
    const int DefaultRecordLength = 4; // bars
    const int MaxRecordLength = 32;
    const int TrackCount = 16;

    // timing relevant stuff
    const int DoublePressTimeout = 250;
    const int HoldTimeout = 333; // ms
    const int DebounceTimeout = 200; // ms
    const int FNResetDelay = 100; // ms
    const int TrackResetDelay = DoublePressTimeout; // ms
    const int RecordLengthDebounceTime = 1000; // ms
    
    const double MaxParameterValue = 255;
    const int KnobSensitivity = 360 / 36;
    const double ParamChangePerStep = MaxParameterValue/ 360 * KnobSensitivity;

    const int CHCount = 2;
    const int CHLeft = 0;
    const int CHRight = 1;
    
    enum TrackID {
        t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15, t16
    };
    
    enum EffectID {
        e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16
    };
    
    enum SampleID {
        s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11, s12, s13, s14, s15, s16
    };
    
    namespace Command {
        enum ID {
            Settings = 1, 
            Function,
            Mute,
            Play,
            Volume,
            Balance,
            Wet,
            ParamA,
            ParamB,
            Record,
            Track1, Track2, Track3, Track4, Track5, Track6, Track7, Track8, Track9, Track10, Track11, Track12, Track13, Track14, Track15, Track16
        };
        
        enum Action {
            Press,
            InstantPress,
            Release,
            Hold,
            DoublePress,
            DoubleHold,
            Increase,
            Decrease,
            Interact,
        };

        enum Trigger {
            Instant,
            OnBeat,
            OnUpBeat
        };
    }

    enum Parameter {
        Bypass,
        Volume,
        Balance,
        Wet,
        ParamA,
        ParamB,
    };

    const juce::String ParameterName[] {
        "Bypass",
        "Volume",
        "Balance",
        "Wet",
        "ParamA",
        "ParamB",
    };

}

