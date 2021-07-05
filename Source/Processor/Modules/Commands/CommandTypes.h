#pragma once

namespace CommandTypes {
    enum Action {
        Press,
        InstantPress,
        Release,
        Hold,
        DoublePress,
        DoubleHold,
        IncreaseValue,
        DecreaseValue,
        Interact,
        
        //TODO: implement ?
        Debounce,
        BeatDebounce
    };

    enum Trigger {
        Instant,
        OnBeat,
        OnUpBeat
    };
}
