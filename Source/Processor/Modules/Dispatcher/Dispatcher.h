#pragma once

#include "Processor/Modules/ControlGroup/ControlGroup.h"
#include "Processor/Modules/Tracks/Track.h"

class SpoolProcessor;

class Dispatcher {
private:
    SpoolProcessor* owner;
    
    
public:
    
    using Action = Track::Action;
    using Mode = Track::Mode;
    using Group = ControlGroup::Group;
    
    enum SelectionRequirement {
        None,
        Selected,
        Unselected
    };

    Dispatcher(SpoolProcessor* owner);
    
    bool single(int index, Action action, Mode mode = Mode::Single, Group group = Group::Unassinged, SelectionRequirement selection = SelectionRequirement::None);
    bool all(Action action, Mode mode = Mode::Single, Group group = Group::Unassinged);
    bool selected(Action action, Mode mode = Mode::Single, Group group = Group::Unassinged);
    bool unselected(Action action, Mode mode = Mode::Single, Group group = Group::Unassinged);
    bool freeOrLastSelected(Action action, Mode mode = Mode::Single);

};
