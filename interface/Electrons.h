#ifndef ELECTRONS_H
#define ELECTRONS_H

#include <vector>
#include <iostream>

#include "TROOT.h"
#include "TLorentzVector.h"

#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "Analysis/CyMiniAna/interface/physicsObjects.h"


// Electrons Class
class Electrons {
  public:
    // Constructor
    explicit Electrons(edm::ParameterSet const& iConfig, edm::ConsumesCollector && iC);

    // Destructor
    virtual ~Electrons();

  private:

    // Container of electrons (defined in interface/physicsObjects.h)
    std::vector<Electron> execute(const edm::Event& evt);

    // Physics information
    std::vector<Electron> m_electrons;

    // Setup physics information from EDMntuples
    float m_elPtMin;
    float m_elAbsEtaMax;
    float m_applyIso;


    // ************
    // Setup to read EDMntuple format
    // -- following this example:
    //    https://github.com/dmajumder/VLQAna
    // ----------member data ---------------------------
    edm::EDGetTokenT<std::vector<float>> t_elCharge;


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
}

#endif
