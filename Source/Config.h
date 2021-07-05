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
    

    const double MaxParamValue = 255;
    const double MinParamValue = 0;
    const int KnobSensitivity = 360 / 36;
    const double ParamChangePerStep = MaxParamValue / 360 * KnobSensitivity;

    namespace Command {
        enum ID {
            Settings = 1,
            Function,
            Mute,
            Play,
            Volume,
            Wet,
            ParamA,
            ParamB,
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

    enum Parameters {
        Bypass,
        Volume,
        Panning,
        Wet,
        ParamA,
        ParamB,
    };

    const juce::String ParameterNames[] {
        "Bypass",
        "Volume",
        "Panning",
        "Wet",
        "ParamA",
        "ParamB",
    };

}

