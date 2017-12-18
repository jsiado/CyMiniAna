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


// Muons Class
class Muons {
  public:
    // Constructor
    explicit Muons(edm::ParameterSet const& iConfig, edm::ConsumesCollector && iC);

    // Destructor
    virtual ~Muons();

  private:

    // Container of Muons (defined in interface/physicsObjects.h)
    std::vector<Muon> execute(const edm::Event& evt);
    std::vector<Muon> execute_truth(const edm::Event& evt);

    // Physics information
    std::vector<Muon> m_muons;
    std::vector<Muon> m_truth_muons;

    // Setup physics information from EDMntuples
    float m_muPtMin;
    float m_muAbsEtaMax;
    float m_muIsoMin;
    float m_muIsoMax;


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
}

#endif
