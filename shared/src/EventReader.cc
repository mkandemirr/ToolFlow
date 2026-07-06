#include "EventReader.h"

bool EventReader::ReadEvent(int eventID)
{
    fEvent.Clear();

    fEvent.eventID = eventID;

    // Fake PMT hits for demonstration.
    // In a real analysis, this method could read hits from a ROOT file.
    fEvent.AddHit(PMTHit(0, 12.5, 3.2));
    fEvent.AddHit(PMTHit(1, 18.0, 1.7));
    fEvent.AddHit(PMTHit(2, 24.3, 5.1));
    fEvent.AddHit(PMTHit(3, 31.8, 2.4));

    return true;
}

const Event& EventReader::GetEvent() const
{
    return fEvent;
}
