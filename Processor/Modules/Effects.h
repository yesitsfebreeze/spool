#pragma once
#include <JuceHeader.h>


class Effects {
public:
    
    template<typename T> struct map_init_helper
    {
        T& data;
        map_init_helper(T& d) : data(d) {}
        map_init_helper& operator() (typename T::key_type const& key, typename T::mapped_type const& value)
        {
            data[key] = value;
            return *this;
        }
    };

    template<typename T> map_init_helper<T> map_init(T& item)
    {
        return map_init_helper<T>(item);
    }

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
    
    
    std::map<Action, const char*> TranslatedActions;
    std::map<Type, const char*> TranslatedTypes;
    
    Effects() {
        map_init(TranslatedActions)
            (On,"On")
            (Off,"Off")
            (Toggle,"Toggle")
            (Invert,"Invert")
        ;

        map_init(TranslatedTypes)
            (Select,"Select")
            (Mute,"Mute")
            (Play,"Play")
        ;
    }
    
    bool doForAllTracks(Type type, Action action) {
        DBG("all : " << TranslatedTypes[type] << " : " << TranslatedActions[action]);
        return true;
    }

    bool doForSelectedTracks(Type type, Action action) {
        DBG("selection : " << TranslatedTypes[type] << " : " << TranslatedActions[action]);
        return true;
    }

    bool doForTrack(int track, Type type, Action action) {
        DBG("track" << track << " : " << TranslatedTypes[type] << " : " << TranslatedActions[action]);
        return true;
    }
};
