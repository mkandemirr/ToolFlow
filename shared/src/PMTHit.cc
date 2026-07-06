#include "PMTHit.h"

#include <iostream>

PMTHit::PMTHit(int pmtID_, double time_, double charge_)
    : pmtID(pmtID_),
      time(time_),
      charge(charge_)
{
}

void PMTHit::Print() const
{
    std::cout << "PMT ID: " << pmtID
              << "  Time: " << time << " ns"
              << "  Charge: " << charge << " PE"
              << std::endl;
}
