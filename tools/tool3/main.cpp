#include <iostream>
#include <string>

#include "INIReader.h"
#include "EventReader.h"

void tool3(const INIReader& iniReader);

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cerr << "Usage: ./bin/tool3 <config.ini>" << std::endl;
        return 1;
    }

    INIReader iniReader(argv[1]);

    if (iniReader.ParseError() != 0) {
        std::cerr << "Error: could not load config file: "
                  << argv[1] << std::endl;
        return 1;
    }

    tool3(iniReader);

    return 0;
}

void tool3(const INIReader& iniReader)
{
    int numberOfEvents =
        iniReader.GetInteger("input", "numberOfEvents", 5);

    EventReader eventReader;

    for (int eventID = 0; eventID < numberOfEvents; ++eventID) {

        if (!eventReader.ReadEvent(eventID)) {
            std::cerr << "Failed to read event "
                      << eventID << std::endl;
            continue;
        }

        const Event& event = eventReader.GetEvent();

        event.Print();

        std::cout << "----------------------------------------"
                  << std::endl;
    }
}
