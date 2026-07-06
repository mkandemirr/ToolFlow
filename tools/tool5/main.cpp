#include <iostream>
#include <string>
#include <vector>

#include "INIReader.h"

#include <TFile.h>
#include <TTree.h>

void tool5(const INIReader& iniReader);

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cerr << "Usage: ./bin/tool5 <config.ini>" << std::endl;
        return 1;
    }

    INIReader iniReader(argv[1]);

    if (iniReader.ParseError() != 0) {
        std::cerr << "Error: could not load config file: "
                  << argv[1] << std::endl;
        return 1;
    }

    tool5(iniReader);

    return 0;
}

void tool5(const INIReader& iniReader)
{
    std::string inputFile =
        iniReader.Get("input", "rootFile", "fakeEvents.root");

    std::string outputFile =
        iniReader.Get("output", "rootFile", "analyzedEvents.root");

    TFile inFile(inputFile.c_str(), "READ");

    if (inFile.IsZombie()) {
        std::cerr << "Error: could not open input file: "
                  << inputFile << std::endl;
        return;
    }

    TTree* inTree = nullptr;
    inFile.GetObject("eventTree", inTree);

    if (!inTree) {
        std::cerr << "Error: could not find tree: eventTree" << std::endl;
        return;
    }

    int eventID;
    double energy;
    int nHits;

    std::vector<int>* pmtID = nullptr;
    std::vector<double>* hitTime = nullptr;
    std::vector<double>* hitCharge = nullptr;

    inTree->SetBranchAddress("eventID", &eventID);
    inTree->SetBranchAddress("energy", &energy);
    inTree->SetBranchAddress("nHits", &nHits);
    inTree->SetBranchAddress("pmtID", &pmtID);
    inTree->SetBranchAddress("hitTime", &hitTime);
    inTree->SetBranchAddress("hitCharge", &hitCharge);

    TFile outFile(outputFile.c_str(), "RECREATE");

    TTree* outTree = inTree->CloneTree(0);

    double totalCharge;
    double meanHitTime;
    double maxCharge;
    double chargePerHit;

    outTree->Branch("totalCharge", &totalCharge);
    outTree->Branch("meanHitTime", &meanHitTime);
    outTree->Branch("maxCharge", &maxCharge);
    outTree->Branch("chargePerHit", &chargePerHit);

    Long64_t numberOfEvents = inTree->GetEntries();

    for (Long64_t i = 0; i < numberOfEvents; ++i) {

        inTree->GetEntry(i);

        totalCharge = 0.0;
        meanHitTime = 0.0;
        maxCharge = 0.0;
        chargePerHit = 0.0;

        if (hitCharge && hitTime && !hitCharge->empty()) {

            for (size_t j = 0; j < hitCharge->size(); ++j) {
                totalCharge += hitCharge->at(j);

                if (hitCharge->at(j) > maxCharge) {
                    maxCharge = hitCharge->at(j);
                }
            }

            for (size_t j = 0; j < hitTime->size(); ++j) {
                meanHitTime += hitTime->at(j);
            }

            meanHitTime /= hitTime->size();
            chargePerHit = totalCharge / hitCharge->size();
        }

        outTree->Fill();
    }

    outTree->Write();
    outFile.Close();
    inFile.Close();

    std::cout << "Input file: " << inputFile << std::endl;
    std::cout << "Output file: " << outputFile << std::endl;
    std::cout << "Processed events: " << numberOfEvents << std::endl;
}
