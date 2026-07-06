#include <iostream>
#include <string>
#include <random>
#include <vector>

#include "INIReader.h"

#include <TFile.h>
#include <TTree.h>

void tool4(const INIReader& iniReader);

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cerr << "Usage: ./bin/tool4 <config.ini>" << std::endl;
        return 1;
    }

    INIReader iniReader(argv[1]);

    if (iniReader.ParseError() != 0) {
        std::cerr << "Error: could not load config file: "
                  << argv[1] << std::endl;
        return 1;
    }

    tool4(iniReader);

    return 0;
}

void tool4(const INIReader& iniReader)
{
    std::string outputFile =
        iniReader.Get("output", "rootFile", "fakeEvents.root");

    int numberOfEvents =
        iniReader.GetInteger("input", "numberOfEvents", 100);

    TFile file(outputFile.c_str(), "RECREATE");
    TTree tree("eventTree", "Fake event tree");

    int eventID;
    double energy;
    int nHits;

    std::vector<int> pmtID;
    std::vector<double> hitTime;
    std::vector<double> hitCharge;

    tree.Branch("eventID", &eventID);
    tree.Branch("energy", &energy);
    tree.Branch("nHits", &nHits);
    tree.Branch("pmtID", &pmtID);
    tree.Branch("hitTime", &hitTime);
    tree.Branch("hitCharge", &hitCharge);

    std::mt19937 rng(12345);

    std::uniform_real_distribution<double> energyDist(100.0, 1000.0);
    std::uniform_int_distribution<int> nHitsDist(5, 20);
    std::uniform_int_distribution<int> pmtIDDist(0, 99);
    std::uniform_real_distribution<double> timeDist(0.0, 200.0);
    std::uniform_real_distribution<double> chargeDist(0.1, 10.0);

    for (int i = 0; i < numberOfEvents; ++i) {

        eventID = i;
        energy = energyDist(rng);
        nHits = nHitsDist(rng);

        pmtID.clear();
        hitTime.clear();
        hitCharge.clear();

        for (int j = 0; j < nHits; ++j) {
            pmtID.push_back(pmtIDDist(rng));
            hitTime.push_back(timeDist(rng));
            hitCharge.push_back(chargeDist(rng));
        }

        tree.Fill();
    }

    tree.Write();
    file.Close();

    std::cout << "Created ROOT file: " << outputFile << std::endl;
    std::cout << "Number of events: " << numberOfEvents << std::endl;
}
