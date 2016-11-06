#ifndef STATUS_EFFECT_HPP_
#define STATUS_EFFECT_HPP_

#include <time.h>
#include <string>
#include <ctime>

enum StatusType {
    UNDEFINED,
    BODYSWAP
};

class StatusEffect {

public:

    StatusEffect(int durationInSeconds);
    time_t getEndTime() const;
    virtual StatusType getType();

private:

    time_t calcEndTime(int duration);
    time_t endTime;

};

class BodySwapStatus : public StatusEffect {

public:

    BodySwapStatus(int durationInSeconds, const std::string& swappedID);
    virtual StatusType getType();
    std::string getSwappedID();

private:

    std::string swappedID;

};

#endif