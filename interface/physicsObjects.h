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
    int isGood;

    void clear(){
        p4.SetPtEtaPhiE(0,0,0,0);
        isGood=false;
    }
};


// Truth information
struct Parton : CmaBase {
    int pdgId;
    int status;
    int index;       // index in vector of truth partons
    int decayIdx;    // index in truth record
    int parent_ref;  // index in truth vector of parent
    int parent_idx;  // index in truth record of parent
    int child0_idx;  // index in truth record of child0
    int child1_idx;  // index in truth record of child1
    int containment;
    int top_index;   // index of parton in the truth_top vector

    // Heavy Object Booleans
    bool isWprime;
    bool isVLQ;

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
    int Top;                    // access the truth top in vector of partons
    int W;                      // access the truth W in vector of partons
    int bottom;                 // access the truth b in vector of partons
    std::vector<int> Wdecays;   // for storing W daughters
    std::vector<int> daughters; // for storing non-W/bottom daughters

    bool isHadronic;  // W decays to quarks
    bool isLeptonic;  // W decays to leptons
};

struct TruthWprime {
    Parton wprime;
    Parton vlq;
    Parton quark;
    Parton vlq_boson;                   // boson from VLQ
    Parton vlq_quark;                   // quark from VLQ
    std::vector<Parton> BosonChildren;  // decays of VLQ boson (W)

    bool isLeptonic;
    bool isHadronic;
};


// Struct for jets
// -- common to all types of jets
struct Jet : CmaBase{
    float bdisc;
    float deepCSV;
    std::map<std::string, bool> isbtagged;
    float charge;

    int index;       // index in vector of jets
    float radius;    // radius of jet (for truth-matching in DeltaR)

    float area;      // area of jet (needed to redo JEC)
    float uncorrPt;  // area of jet (needed to redo JEC)
    float uncorrE;   // area of jet (needed to redo JEC)

    bool isHadTop;   // matched to hadronically-decaying truth top parton
    int true_flavor; // true flavor of jet (quark/gluon match)
    int truth_jet;   // index in vector of truth jets that is matched to this jet
    int containment; // level of containment for partons matched to jet
    int matchId;     // keep track if jet is matched to top or anti-top
    std::vector<int> truth_partons;  // vector containing partons that are truth-matched to jet
};

struct Ljet : Jet{
    // extra ljet attributes
    float tau1;
    float tau2;
    float tau3;
    float tau21;
    float tau32;
    float softDropMass;

    float BEST_t;
    float BEST_w;
    float BEST_z;
    float BEST_h;
    float BEST_j;
    float BEST_class;

    float charge;
    std::vector<Jet> subjets;
    float subjet0_charge;
    float subjet0_bdisc;
    float subjet0_mass;
    float subjet0_pt;
    float subjet0_tau1;
    float subjet0_tau2;
    float subjet0_tau3;
    float subjet1_charge;
    float subjet1_bdisc;
    float subjet1_mass;
    float subjet1_pt;
    float subjet1_tau1;
    float subjet1_tau2;
    float subjet1_tau3;

    int target;
    std::map<std::string, double> features;  // store features in map to easily access later
    std::map<std::string, double> dnn;       // store full dnn results
};



// Extra lepton attributes
struct Lepton : CmaBase{
    // common to electrons and muons
    int charge;
    bool isElectron;
    bool isMuon;
    int index;       // index in vector of leptons

    float drmin;     // distance to closest AK4
    float ptrel;     // relative pT to closest AK4
    float iso;
    float id;
    int loose;
    int medium;
    int tight;
    int loose_noIso;
    int medium_noIso;
    int tight_noIso;
};

struct Electron : Lepton{
    // extra electron attributes
    Electron() {
        isElectron = true;
        isMuon     = false;
    }
};
struct Muon : Lepton{
    // extra muon attributes
    Muon() {
        isElectron = false;
        isMuon     = true;
    }
};

struct Neutrino : CmaBase{
    // extra neutrino attributes
    float pz_sampling;
    std::vector<float> pz_samplings;
};

struct MET : CmaBase{
    // extra MET attributes
    float mtw;   // transverse mass of W
};


// VLQ (assuming T->bW)
struct VLQ : CmaBase{
};

// Wprime (assuming W'->bT for now)
struct Wprime : CmaBase{
    VLQ vlq;
    Jet jet;
};




// ------------------------ // 
// Struct to contain sample information (processing the input file)

struct Sample {
    std::string sampleType;        // kind of sample, e.g., 'ttbar', 'qcd', 'signal', etc.
    std::string primaryDataset;    // primary dataset (how to identify the sample & metadata)
    float XSection;
    float KFactor;
    float sumOfWeights;
    unsigned int NEvents;

    void clear(){
        sampleType = "";
        primaryDataset = "";
        XSection = 1.;
        KFactor  = 1.;
        sumOfWeights = 1.;
        NEvents = 1;
    }
};


#endif
