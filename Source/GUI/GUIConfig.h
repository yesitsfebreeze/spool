#pragma once

#include <JuceHeader.h>


// color scheme
// https://coolors.co/82ddf5-ffe95e-78ffa3-ff5452-1d1c22-414148-e9e9ed-ad67ef-eda152

namespace GUIConfig {

    const int WindowAspectRatio = 1;
    const int DefaultWindowSize = 900;
    const int MinimumWindowSize = 700;

    const int BorderRadius = 6;
    const int BorderWidth = 4;
    const int Padding = 20;

    const int TrackColumns = 4;
    const int TrackRows = 4;

    const float DotSize = 0.6;

    const float FPS = 30;

    namespace Colors {
        const juce::Colour Light = juce::Colour { 0xffe9e9ed };
        const juce::Colour Mid = juce::Colour { 0xff414148 };
        const juce::Colour Dark = juce::Colour { 0xff1D1C22 };
        const juce::Colour Blue = juce::Colour { 0xff82DDF5 };
        const juce::Colour Yellow = juce::Colour { 0xffFFE95E };
        const juce::Colour Green = juce::Colour { 0xff78FFA3 };
        const juce::Colour Red = juce::Colour { 0xffFF5452 };

        const juce::Colour GroupColorOne = juce::Colour { 0xffad67ef };
        const juce::Colour GroupColorTwo = juce::Colour { 0xffeda152 };

        // peakmeter
        const juce::Colour PeakOne = juce::Colour { 0xffB3FA5C };
        const juce::Colour PeakTwo = juce::Colour { 0xffFAF75C };
        const juce::Colour PeakThree = juce::Colour { 0xffFAB35C };
        const juce::Colour PeakFour = juce::Colour { 0xffFA6C5C };

        // automatic variants
        const juce::Colour LightBlue = Blue.interpolatedWith(Light, 0.5);
        const juce::Colour MidBlue = Blue.interpolatedWith(Mid, 0.5);
        const juce::Colour DarkBlue = Blue.interpolatedWith(Dark, 0.5);
        const juce::Colour LightYellow = Yellow.interpolatedWith(Light, 0.5);
        const juce::Colour MidYellow = Yellow.interpolatedWith(Mid, 0.5);
        const juce::Colour DarkYellow = Yellow.interpolatedWith(Dark, 0.5);
        const juce::Colour LightGreen = Green.interpolatedWith(Light, 0.5);
        const juce::Colour MidGreen = Green.interpolatedWith(Mid, 0.5);
        const juce::Colour DarkGreen = Green.interpolatedWith(Dark, 0.5);
        const juce::Colour LightRed = Red.interpolatedWith(Light, 0.5);
        const juce::Colour MidRed = Red.interpolatedWith(Mid, 0.5);
        const juce::Colour DarkRed = Red.interpolatedWith(Dark, 0.5);
    }

    namespace DefaultKeyBind {
        const int Settings = 44;
        const int Function = 72;
        const int Mute = 74;
        const int Play = 75;
        const int Record = 76;
        const int Track1 = 49;
        const int Track2 = 50;
        const int Track3 = 51;
        const int Track4 = 52;
        const int Track5 = 81;
        const int Track6 = 87;
        const int Track7 = 69;
        const int Track8 = 82;
        const int Track9 = 65;
        const int Track10 = 83;
        const int Track11 = 68;
        const int Track12 = 70;
        const int Track13 = 89;
        const int Track13Alt = 90;
        const int Track14 = 88;
        const int Track15 = 67;
        const int Track16 = 86;
    }

    namespace SettingsWindow {
        const int SettingsWindowWidth = 600;
        const int SettingsWindowMinWidth = 400;
        const int SettingsWindowHeight = 800;
        const int SettingsWindowMinHeight = 400;
    }

}


