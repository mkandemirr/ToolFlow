#include "Event.h"

#include <iostream>

void Event::AddHit(const PMTHit& hit)
{
    pmtHits.push_back(hit);
}

void Event::Clear()
{
    eventID = -1;
    pmtHits.clear();
}

void Event::Print() const
{
    std::cout << "Event ID: " << eventID << std::endl;
    std::cout << "Number of PMT hits: " << pmtHits.size() << std::endl;

    for (const auto& hit : pmtHits) {
        hit.Print();
    }
}
