#ifndef PMT_HIT_H
#define PMT_HIT_H

class PMTHit
{
public:
    PMTHit() = default;

    PMTHit(int pmtID, double time, double charge);

    void Print() const;

public:
    int    pmtID  = -1;
    double time   = 0.0;   // ns
    double charge = 0.0;   // p.e.
};

#endif
