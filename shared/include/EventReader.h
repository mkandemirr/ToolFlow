#ifndef EVENT_READER_H
#define EVENT_READER_H

#include "Event.h"

class EventReader
{
public:
    EventReader() = default;

    bool ReadEvent(int eventID);

    const Event& GetEvent() const;

private:
    Event fEvent;
};

#endif
