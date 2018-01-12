#ifndef MUONS_H
#define MUONS_H

#include <vector>
#include <iostream>

#include "TROOT.h"
#include "TLorentzVector.h"

#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "Analysis/CyMiniAna/interface/physicsObjects.h"
#include "Analysis/CyMiniAna/interface/objectSelection.h"

// Muons Class
class Muons {
  public:
    // Constructor
    explicit Muons(edm::ParameterSet const& iConfig, edm::ConsumesCollector && iC);

    // Destructor
    virtual ~Muons();

  private:

    // Container of Muons (defined in interface/physicsObjects.h)
    std::vector<Muon> execute(const edm::Event& evt, const objectSelection& obj);
    std::vector<Muon> execute_truth(const edm::Event& evt, const objectSelection& obj);

    // Physics information
    std::vector<Muon> m_muons;
    std::vector<Muon> m_truth_muons;

    // Setup physics information from EDMntuples
    bool m_useTruth;


    // ************
    // Setup to read EDMntuple format
    // -- following this example:
    //    https://github.com/dmajumder/VLQAna
    // ----------member data ---------------------------
    // Muon handles
    edm::Handle<std::vector<float>> h_muCharge;
    edm::Handle<std::vector<float>> h_muDxy;
    edm::Handle<std::vector<float>> h_muDz;
    edm::Handle<std::vector<float>> h_muE;
    edm::Handle<std::vector<float>> h_muEta;
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

    // Truth Muons
    edm::Handle<std::vector<float>> h_muGenMuonCharge;
    edm::Handle<std::vector<float>> h_muGenMuonE;
    edm::Handle<std::vector<float>> h_muGenMuonEta;
    edm::Handle<std::vector<float>> h_muGenMuonPhi;
    edm::Handle<std::vector<float>> h_muGenMuonPt;

    // Tokens
    edm::EDGetTokenT<std::vector<float>> t_muCharge;
    edm::EDGetTokenT<std::vector<float>> t_muDxy;
    edm::EDGetTokenT<std::vector<float>> t_muDz;
    edm::EDGetTokenT<std::vector<float>> t_muE;
    edm::EDGetTokenT<std::vector<float>> t_muEta;
    edm::EDGetTokenT<std::vector<float>> t_muGlbTrkNormChi2;
    edm::EDGetTokenT<std::vector<float>> t_muInTrkNormChi2;
    edm::EDGetTokenT<std::vector<float>> t_muIsGlobalMuon;
    edm::EDGetTokenT<std::vector<float>> t_muIsLooseMuon;
    edm::EDGetTokenT<std::vector<float>> t_muIsPFMuon;
    edm::EDGetTokenT<std::vector<float>> t_muIsSoftMuon;
    edm::EDGetTokenT<std::vector<float>> t_muIsTightMuon;
    edm::EDGetTokenT<std::vector<float>> t_muIsTrackerMuon;
    edm::EDGetTokenT<std::vector<float>> t_muIso04;
    edm::EDGetTokenT<std::vector<float>> t_muKey;
    edm::EDGetTokenT<std::vector<float>> t_muNumberMatchedStations;
    edm::EDGetTokenT<std::vector<float>> t_muNumberOfPixelLayers;
    edm::EDGetTokenT<std::vector<float>> t_muNumberOfValidTrackerHits;
    edm::EDGetTokenT<std::vector<float>> t_muNumberTrackerLayers;
    edm::EDGetTokenT<std::vector<float>> t_muNumberValidMuonHits;
    edm::EDGetTokenT<std::vector<float>> t_muNumberValidPixelHits;
    edm::EDGetTokenT<std::vector<float>> t_muPhi;
    edm::EDGetTokenT<std::vector<float>> t_muPt;
    edm::EDGetTokenT<std::vector<float>> t_muSumChargedHadronPt;
    edm::EDGetTokenT<std::vector<float>> t_muSumNeutralHadronPt;
    edm::EDGetTokenT<std::vector<float>> t_muSumPUPt;
    edm::EDGetTokenT<std::vector<float>> t_muSumPhotonPt;
    // Truth Muons
    edm::EDGetTokenT<std::vector<float>> t_muGenMuonCharge;
    edm::EDGetTokenT<std::vector<float>> t_muGenMuonE;
    edm::EDGetTokenT<std::vector<float>> t_muGenMuonEta;
    edm::EDGetTokenT<std::vector<float>> t_muGenMuonPhi;
    edm::EDGetTokenT<std::vector<float>> t_muGenMuonPt;
}

#endif
