#ifndef PHYSICSOBJECTS_H
#define PHYSICSOBJECTS_H

/* 
   Physics objects to be used in analyses
*/
#include "TLorentzVector.h"
#include <map>
#include <string>


// easily keep track of isolation and ID working points
enum class lep_id  {LOOSE, MEDIUM, TIGHT, VETO, NONE};
enum class lep_iso {LOOSE, MEDIUM, TIGHT, NONE};
enum class jet_id  {LOOSE, MEDIUM, TIGHT, TIGHTLEPVETO, NONE};


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
    int true_flavor;
    float btagSF;
    float btagSF_UP;
    float btagSF_DN;

    float partonFlavour;
    float hadronFlavour;
    float nMultip;
    float nHadEnergy;
    float nEMEnergy;
    float cHadEnergy;
    float cEMEnergy;
    float cMultip;
    float muonEnergy;

    float jecFactor0;
    float jetArea;
    float ptResolution;
    float smearedPt;

    float JECsyst;
    float JERSF;
    float JERSF_UP;
    float JERSF_DN;

    std::vector<int> keys;

    float charge;
    int index;    // index in vector of jets

    bool loose;
    bool medium;
    bool tight;
    bool tightlepveto;
};

struct Ljet : Jet{
    // extra ljet attributes
    int isGood;
    // substructure
    float tau1;
    float tau2;
    float tau3;
    float tau21;
    float tau32;
    float softDropMass;
    std::vector<Jet> subjets;
    float charge;
    float vSubjetIndex0;
    float vSubjetIndex1;
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
    bool loose;
    bool medium;
    bool tight;
};

struct Electron : Lepton{
    // extra electron attributes
    Electron() {
        isElectron = true;
        isMuon     = false;
    }

    float charge;
    float iso03;
    float iso03db;
    float SCEta;
    float SCPhi;
    bool vidHEEP;
    bool vidHEEPnoiso;
    bool vidVeto;
    bool vidVetonoiso;
    bool vidLoose;
    bool vidLoosenoiso;
    bool vidMedium;
    bool vidMediumnoiso;
    bool vidTight;
    bool vidTightnoiso;
    float vidMvaGPvalue;
    float vidMvaGPcateg;
    float vidMvaHZZvalue;
    float vidMvaHZZcateg;

    float vetoSF;
    float looseSF;
    float mediumSF;
    float tightSF;
    float recoSF;
    float vetoSF_UP;
    float looseSF_UP;
    float mediumSF_UP;
    float tightSF_UP;
    float recoSF_UP;
    float vetoSF_DN;
    float looseSF_DN;
    float mediumSF_DN;
    float tightSF_DN;
    float recoSF_DN;

    // electron ID variables (to re-calculate)
    float Dz;
    float Dxy;
    float HoE;
    float scEta;
    float dPhiIn;
    float ooEmooP;
    int missHits;
    float RelIsoEA;
    float dEtaInSeed;
    float full5x5siee;
    bool hasMatchedConVeto;


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

    float mediumID_SF;
    float mediumID_SF_UP;
    float mediumID_SF_DN;
    float looseISO_SF;
    float looseISO_SF_UP;
    float looseISO_SF_DN;
    float trigger_SF;
    float trigger_SF_UP;
    float trigger_SF_DN;
    float track_SF;
    float track_SF_UP;
    float track_SF_DN;
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


// Truth information
struct Parton : CmaBase {
    int pdgId;
    int index;       // index in vector of truth partons
    int decayIdx;    // index in truth record
    int parent_ref;  // index in truth vector of parent
    int parent_idx;  // index in truth record of parent
    int top_index;   // index in truth_tops if this is a top
    int containment; // record value used to calculate containment

    // Heavy Object Booleans
    bool isTop;
    bool isW;
    bool isZ;
    bool isHiggs;
    // Lepton Booleans
    bool isLepton;
    bool isTau;
    bool isElectron;
    bool isMuon;
    bool isNeutrino;
    // Quark Booleans
    bool isQuark;
    bool isBottom;
    bool isLight;
};

struct TruthTop {
    // collect indices in truth_partons vector of top parton info
    bool isTop;
    bool isAntiTop;
    int Top;
    int W;
    int bottom;
    std::vector<int> Wdecays;   // for storing W daughters
    std::vector<int> daughters; // for storing non-W/bottom daughters

    bool isHadronic;  // W decays to quarks
    bool isLeptonic;  // W decays to leptons
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

