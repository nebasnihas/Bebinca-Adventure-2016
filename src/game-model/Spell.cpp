//
// Created by pmadeya on 31/10/16.
//

#include "../../include/game/Spell.hpp"


Spell::Spell(const std::string &effect, int duration, const std::string &name)
    : effect(effect)
    , duration(duration)
    , name(name)
{}

const std::string &Spell::getEffect() const {
    return effect;
}

int Spell::getDuration() const {

    assert(duration >= 0);
    return duration;
}

const std::string &Spell::getName() const {
    assert(name.compare(""));
    return name;
}
