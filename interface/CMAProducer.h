#ifndef CMAPRODUCER_H
#define CMAPRODUCER_H

#include <set>
#include <list>
#include <memory>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TSystem.h"
#include "TLorentzVector.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "Analysis/CyMiniAna/interface/physicsObjects.h"


// CMAProducer Class
class CMAProducer : public edm::EDProducer {
  public:
    // Constructor
    explicit CMAProducer(const edm::ParameterSet&);

    // Destructor
    virtual ~CMAProducer();

  private:

    virtual void beginJob(const edm::EventSetup&);
    virtual void produce(edm::Event&, const edm::EventSetup&);
    virtual void endJob();

    // Build physics objects
    void initialize_electrons(edm::Event& evt);
    void initialize_muons(edm::Event& evt);
    void initialize_neutrinos(edm::Event& evt);
    void initialize_jets(edm::Event& evt);
    void initialize_largeRjets(edm::Event& evt);
    void initialize_kinematics(edm::Event& evt);

    // General Parameters
    bool m_isMC;
    bool m_cleanFlags;
    bool m_useTruth;
    bool m_useJets;
    bool m_useLargeRJets;
    bool m_useLeptons;
    bool m_useNeutrinos;
    bool m_kinematicReco;
    std::string m_metadataFile;
    float m_LUMI;

    // Physics information
    std::vector<Electron> m_electrons;
    std::vector<Muon> m_muons;
    std::vector<Neutrino> m_neutrinos;
    std::vector<Jet> m_jets;
    std::vector<Ljet> m_ljets;
    float m_HT;
    float m_ST;

    // Setup physics information from EDMntuples
    float m_elPtMin;
    float m_elAbsEtaMax;
    float m_applyIso;
    float m_muPtMin;
    float m_muAbsEtaMax;
    float m_muIsoMin;
    float m_muIsoMax;
    float m_jetPtMin;
    float m_jetAbsEtaMax;
    float m_ljetPtMin;
    float m_ljetAbsEtaMax;
    float m_METPtMin;

    // ************
    // Setup to read EDMntuple format
    // -- following this example:
    //    https://github.com/dmajumder/VLQAna
    // ----------member data ---------------------------
    edm::ConsumesCollector& m_iC; // = consumesCollector();
    edm::InputTag m_src;

    edm::EDGetTokenT<float> t_rho;
    edm::EDGetTokenT<int> t_runno;
    edm::EDGetTokenT<int> t_lumisec;
    edm::EDGetTokenT<int> t_evtno;
    edm::EDGetTokenT<bool> t_isData;
    edm::EDGetTokenT<bool> t_hltdecision;
    edm::EDGetTokenT<std::string> t_evttype;
    edm::EDGetTokenT<float> t_evtwtGen;
    edm::EDGetTokenT<float> t_evtwtPV;
    edm::EDGetTokenT<float> t_evtwtPVBG;
    edm::EDGetTokenT<float> t_evtwtPVH;
    edm::EDGetTokenT<float> t_evtwtPVLow;
    edm::EDGetTokenT<float> t_evtwtPVHigh;
    edm::EDGetTokenT<unsigned int> t_npv;
    edm::EDGetTokenT<int> t_npuTrue;
    edm::EDGetTokenT<float> t_htHat;
    edm::EDGetTokenT<std::vector<int>> t_lhewtids;
    edm::EDGetTokenT<std::vector<float>> t_lhewts;

    // Electron handles
    edm::Handle<std::vector<float>> h_elCharge;
    edm::Handle<std::vector<float>> h_elDxy;
    edm::Handle<std::vector<float>> h_elDz;
    edm::Handle<std::vector<float>> h_elE;
    edm::Handle<std::vector<float>> h_elEta;
    edm::Handle<std::vector<float>> h_elHoE;
    edm::Handle<std::vector<float>> h_elRelIsoEA;
    edm::Handle<std::vector<float>> h_elKey;
    edm::Handle<std::vector<float>> h_elPhi;
    edm::Handle<std::vector<float>> h_elPt;
    edm::Handle<std::vector<float>> h_eldEtaIn;
    edm::Handle<std::vector<float>> h_eldEtaInSeed;
    edm::Handle<std::vector<float>> h_eldPhiIn;
    edm::Handle<std::vector<float>> h_elfull5x5siee;
    edm::Handle<std::vector<float>> h_elhasMatchedConVeto;
    edm::Handle<std::vector<float>> h_elvidLoose;
    edm::Handle<std::vector<float>> h_elvidMedium;
    edm::Handle<std::vector<float>> h_elvidTight;
    edm::Handle<std::vector<float>> h_elvidVeto;
    edm::Handle<std::vector<float>> h_elvidHEEP;
    edm::Handle<std::vector<float>> h_elmissHits;
    edm::Handle<std::vector<float>> h_elooEmooP;
    edm::Handle<std::vector<float>> h_elscEta;

    // Muon handles
    edm::Handle<std::vector<float>> h_muCharge;
    edm::Handle<std::vector<float>> h_muDxy;
    edm::Handle<std::vector<float>> h_muDz;
    edm::Handle<std::vector<float>> h_muE;
    edm::Handle<std::vector<float>> h_muEta;
    edm::Handle<std::vector<float>> h_muGenMuonCharge;
    edm::Handle<std::vector<float>> h_muGenMuonE;
    edm::Handle<std::vector<float>> h_muGenMuonEta;
    edm::Handle<std::vector<float>> h_muGenMuonPhi;
    edm::Handle<std::vector<float>> h_muGenMuonPt;
    edm::Handle<std::vector<float>> h_muGlbTrkNormChi2;
    edm::Handle<std::vector<float>> h_muInTrkNormChi2;
    edm::Handle<std::vector<float>> h_muIsGlobalMuon;
    edm::Handle<std::vector<float>> h_muIsLooseMuon;
    edm::Handle<std::vector<float>> h_muIsPFMuon;
    edm::Handle<std::vector<float>> h_muIsSoftMuon;
    edm::Handle<std::vector<float>> h_muIsTightMuon;
    edm::Handle<std::vector<float>> h_muIsTrackerMuon;
    edm::Handle<std::vector<float>> h_muIso04;
    edm::Handle<std::vector<float>> h_muKey;
    edm::Handle<std::vector<float>> h_muNumberMatchedStations;
    edm::Handle<std::vector<float>> h_muNumberOfPixelLayers;
    edm::Handle<std::vector<float>> h_muNumberOfValidTrackerHits;
    edm::Handle<std::vector<float>> h_muNumberTrackerLayers;
    edm::Handle<std::vector<float>> h_muNumberValidMuonHits;
    edm::Handle<std::vector<float>> h_muNumberValidPixelHits;
    edm::Handle<std::vector<float>> h_muPhi;
    edm::Handle<std::vector<float>> h_muPt;
    edm::Handle<std::vector<float>> h_muSumChargedHadronPt;
    edm::Handle<std::vector<float>> h_muSumNeutralHadronPt;
    edm::Handle<std::vector<float>> h_muSumPUPt;
    edm::Handle<std::vector<float>> h_muSumPhotonPt;


    // AK4 Jet Handles
    edm::Handle <int> h_npv;
    edm::Handle <float> h_rho;
    edm::Handle <std::vector<float>> h_jetPt;
    edm::Handle <std::vector<float>> h_jetEta;
    edm::Handle <std::vector<float>> h_jetPhi;
    edm::Handle <std::vector<float>> h_jetEnergy;
    edm::Handle <std::vector<float>> h_jetHadronFlavour;
    edm::Handle <std::vector<float>> h_jetPartonFlavour;
    edm::Handle <std::vector<float>> h_jetCSV;
    edm::Handle <std::vector<float>> h_jetCMVA;
    edm::Handle <std::vector<float>> h_jetCvsB;
    edm::Handle <std::vector<float>> h_jetCvsL;
    edm::Handle <std::vector<float>> h_jetJEC;
    edm::Handle <std::vector<float>> h_jetnHadEnergy;
    edm::Handle <std::vector<float>> h_jetnEMEnergy;
    edm::Handle <std::vector<float>> h_jetcHadEnergy;
    edm::Handle <std::vector<float>> h_jetcEMEnergy;
    edm::Handle <std::vector<float>> h_jetcMultip;
    edm::Handle <std::vector<float>> h_jetnMultip;
    edm::Handle <std::vector<float>> h_jetY;
    edm::Handle <std::vector<float>> h_jetArea;
    // Truth jets
    edm::Handle <std::vector<float>> h_jetGenJetE;
    edm::Handle <std::vector<float>> h_jetGenJetEta;
    edm::Handle <std::vector<float>> h_jetGenJetPt;
    edm::Handle <std::vector<float>> h_jetGenJetPhi;

    // AK8 Jet Handles
    edm::EDGetTokenT<std::vector<float>> t_ljetPt;
    edm::EDGetTokenT<std::vector<float>> t_ljetEta;
    edm::EDGetTokenT<std::vector<float>> t_ljetPhi;
    edm::EDGetTokenT<std::vector<float>> t_ljetMass;
    edm::EDGetTokenT<std::vector<float>> t_ljetEnergy;
    edm::EDGetTokenT<std::vector<float>> t_ljetPartonFlavour;
    edm::EDGetTokenT<std::vector<float>> t_ljetHadronFlavour;
    edm::EDGetTokenT<std::vector<float>> t_ljetCSV;
    edm::EDGetTokenT<std::vector<float>> t_ljetCMVA;
    edm::EDGetTokenT<std::vector<float>> t_ljetCvsB;
    edm::EDGetTokenT<std::vector<float>> t_ljetCvsL;
    edm::EDGetTokenT<std::vector<float>> t_ljetJEC;
    edm::EDGetTokenT<std::vector<float>> t_ljetnHadEnergy;
    edm::EDGetTokenT<std::vector<float>> t_ljetnEMEnergy;
    edm::EDGetTokenT<std::vector<float>> t_ljetHFHadronEnergy;
    edm::EDGetTokenT<std::vector<float>> t_ljetcHadEnergy;
    edm::EDGetTokenT<std::vector<float>> t_ljetcEMEnergy;
    edm::EDGetTokenT<std::vector<float>> t_ljetnumDaughters;
    edm::EDGetTokenT<std::vector<float>> t_ljetcMultip;
    edm::EDGetTokenT<std::vector<float>> t_ljetnMultip;
    edm::EDGetTokenT<std::vector<float>> t_ljetY;
    edm::EDGetTokenT<std::vector<float>> t_ljetArea;
    edm::EDGetTokenT<std::vector<float>> t_ljettau1;
    edm::EDGetTokenT<std::vector<float>> t_ljettau2;
    edm::EDGetTokenT<std::vector<float>> t_ljettau3;
    edm::EDGetTokenT<std::vector<float>> t_ljetPrunedMass;
    edm::EDGetTokenT<std::vector<float>> t_ljetTrimmedMass;
    edm::EDGetTokenT<std::vector<float>> t_ljetFilteredMass;
    edm::EDGetTokenT<std::vector<float>> t_ljetSoftDropMass;
    edm::EDGetTokenT<std::vector<float>> t_ljetnSubJets;
    edm::EDGetTokenT<std::vector<float>> t_ljet_subjetIdx0;
    edm::EDGetTokenT<std::vector<float>> t_ljet_subjetIdx1;
    edm::EDGetTokenT<std::vector<float>> t_ljet_subjetHadFl;
    edm::EDGetTokenT<std::vector<float>> t_ljet_subjetPt;
    edm::EDGetTokenT<std::vector<float>> t_ljet_subjetEta;
    edm::EDGetTokenT<std::vector<float>> t_ljet_subjetPhi;
    edm::EDGetTokenT<std::vector<float>> t_ljet_subjetMass;
    edm::EDGetTokenT<std::vector<float>> t_ljet_subjetEnergy;
    edm::EDGetTokenT<std::vector<float>> t_ljet_subjetJEC;
    edm::EDGetTokenT<std::vector<float>> t_ljet_subjetCSV;
    edm::EDGetTokenT<std::vector<float>> t_ljet_subjetCMVA;
    // Truth jets
    edm::EDGetTokenT<std::vector<float>> t_ljetGenJetPt;
    edm::EDGetTokenT<std::vector<float>> t_ljetGenJetEta;
    edm::EDGetTokenT<std::vector<float>> t_ljetGenJetPhi;
    edm::EDGetTokenT<std::vector<float>> t_ljetGenJetE;
    edm::EDGetTokenT<std::vector<float>> t_ljetGenJetCharge;
    edm::EDGetTokenT<std::vector<float>> t_ljet_subjetGenJetPt;
    edm::EDGetTokenT<std::vector<float>> t_ljet_subjetGenJetEta;
    edm::EDGetTokenT<std::vector<float>> t_ljet_subjetGenJetPhi;
    edm::EDGetTokenT<std::vector<float>> t_ljet_subjetGenJetE;
    edm::EDGetTokenT<std::vector<float>> t_ljet_subjetGenJetCharge; 

    // MET
    edm::Handle<std::vector<float>> h_metFullPhi;
    edm::Handle<std::vector<float>> h_metFullPt;
    edm::Handle<std::vector<float>> h_metFullPx;
    edm::Handle<std::vector<float>> h_metFullPy;
    edm::Handle<std::vector<float>> h_metFulluncorPhi;
    edm::Handle<std::vector<float>> h_metFulluncorPt;
    edm::Handle<std::vector<float>> h_metFulluncorSumEt;
};

#endif
