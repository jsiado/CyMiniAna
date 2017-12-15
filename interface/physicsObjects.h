#ifndef PHYSICSOBJECTS_H_
#define PHYSICSOBJECTS_H_

/* 
   Physics objects to be used in analyses
   This structure allows the Event class
   and other classes to access these objects
   without circular inclusion (which breaks!)
*/
#include "TLorentzVector.h"
#include <map>
#include <string>

// base object (consistent reference to TLorentzVector)
struct CmaBase {
    TLorentzVector p4;
};


// Struct for particle flow jets
// -- common to all types of jets
struct Jet : CmaBase{
    float cMVAv2;
    float CSVv2;
    float CvsL;
    float CvsB;
    std::map<std::string, bool> isbtagged;

    float partonFlavour;
    float hadronFlavour;
    float neutralMultiplicity;
    float neutralHadronEnergyFrac;
    float neutralEmEnergyFrac;
    float chargedHadronEnergyFrac;
    float chargedEmEnergyFrac;
    float chargedMultiplicity;

    float jecFactor0;
    float jetArea;
    float ptResolution;
    float smearedPt;
    std::vector<int> keys;

    float charge;
    int index;    // index in vector of jets
};

struct Ljet : Jet{
    // extra ljet attributes
    int isGood;
    float tau1_CHS;
    float tau2_CHS;
    float tau3_CHS;
    float tau21_CHS;
    float tau32_CHS;
    float softDropMass_CHS;
    float vSubjetIndex0;
    float vSubjetIndex1;
    float charge;
};

struct Tjet : Jet{
    // Track Jets not used in CMS -- here as a placeholder
    int numConstituents;
};



// Extra lepton attributes
struct Lepton : CmaBase{
    // common to electrons and muons
    int charge;
    bool isElectron;
    bool isMuon;
    int index;       // index in vector of leptons

    float key;
    float miniIso;
    float loose;
    float medium;
    float tight;
};

struct Electron : Lepton{
    // extra electron attributes
    Electron() {
        isElectron = true;
        isMuon     = false;
    }

    float iso03;
    float iso03db;
    float SCEta;
    float SCPhi;
    float vidVeto;
    float vidLoose;
    float vidMedium;
    float vidTight;
    float vidHEEP;
    float vidVetonoiso;
    float vidLoosenoiso;
    float vidMediumnoiso;
    float vidTightnoiso;
    float vidHEEPnoiso;
    float vidMvaGPvalue;
    float vidMvaGPcateg;
    float vidMvaHZZvalue;
    float vidMvaHZZcateg;
    int veto_NoIsoID;
    int loose_NoIsoID;
    int medium_NoIsoID;
    int tight_NoIsoID;
    int isoVeto;
    int isoLoose;
    int isoMedium;
    int isoTight;
    int vetoID;
};
struct Muon : Lepton{
    // extra muon attributes
    Muon() {
        isElectron = false;
        isMuon     = true;
    }

    float iso04;
    float soft;
    float medium2016;
    float hightPt;
};

struct Neutrino : CmaBase{
    // extra neutrino attributes
};

struct MET : CmaBase{
    // extra MET attributes
    float uncorrPhi;
    float uncorrPt;
    float uncorrSumEt;
};



// VLQ (assuming T->bW)
struct VLQ : CmaBase{
};

// Wprime (assuming W'->bT for now)
struct Wprime : CmaBase{
    VLQ vlq;
    Jet jet;
};

#endif

