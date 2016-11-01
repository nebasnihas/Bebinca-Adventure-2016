//
// Created by pmadeya on 31/10/16.
//

#ifndef SPELL_HPP
#define SPELL_HPP

#include <string>
#include <assert.h>

class Spell
{

private:
    int duration;
    std::string effect;
    std::string name;



public:

    Spell(const std::string &effect, int duration, const std::string &name);

    const std::string &getEffect() const;
    int getDuration() const;
    const std::string &getName() const;


};

#endif
