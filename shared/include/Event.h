#ifndef EVENT_H
#define EVENT_H

#include "PMTHit.h"

#include <vector>

class Event
{
public:
    Event() = default;

    void AddHit(const PMTHit& hit);

    void Clear();

    void Print() const;

public:
    int eventID = -1;

    std::vector<PMTHit> pmtHits;
};

#endif
