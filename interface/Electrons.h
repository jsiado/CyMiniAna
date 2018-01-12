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
#include "Analysis/CyMiniAna/interface/objectSelection.h"

// Electrons Class
class Electrons {
  public:
    // Constructor
    explicit Electrons(edm::ParameterSet const& iConfig, edm::ConsumesCollector && iC);

    // Destructor
    virtual ~Electrons();

  private:

    // Container of electrons (defined in interface/physicsObjects.h)
    std::vector<Electron> execute(const edm::Event& evt, const objectSelection& obj);

    // Physics information
    std::vector<Electron> m_electrons;

    // Setup physics information from EDMntuples


    // ************
    // Setup to read EDMntuple format
    // -- following this example:
    //    https://github.com/dmajumder/VLQAna
    // ----------member data ---------------------------
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

    // Tokens
    edm::EDGetTokenT<std::vector<float>> t_elCharge;
    edm::EDGetTokenT<std::vector<float>> t_elDxy;
    edm::EDGetTokenT<std::vector<float>> t_elDz;
    edm::EDGetTokenT<std::vector<float>> t_elE;
    edm::EDGetTokenT<std::vector<float>> t_elEta;
    edm::EDGetTokenT<std::vector<float>> t_elHoE;
    edm::EDGetTokenT<std::vector<float>> t_elRelIsoEA;
    edm::EDGetTokenT<std::vector<float>> t_elKey;
    edm::EDGetTokenT<std::vector<float>> t_elPhi;
    edm::EDGetTokenT<std::vector<float>> t_elPt;
    edm::EDGetTokenT<std::vector<float>> t_eldEtaIn;
    edm::EDGetTokenT<std::vector<float>> t_eldEtaInSeed;
    edm::EDGetTokenT<std::vector<float>> t_eldPhiIn;
    edm::EDGetTokenT<std::vector<float>> t_elfull5x5siee;
    edm::EDGetTokenT<std::vector<float>> t_elhasMatchedConVeto;
    edm::EDGetTokenT<std::vector<float>> t_elvidLoose;
    edm::EDGetTokenT<std::vector<float>> t_elvidMedium;
    edm::EDGetTokenT<std::vector<float>> t_elvidTight;
    edm::EDGetTokenT<std::vector<float>> t_elvidVeto;
    edm::EDGetTokenT<std::vector<float>> t_elvidHEEP;
    edm::EDGetTokenT<std::vector<float>> t_elmissHits;
    edm::EDGetTokenT<std::vector<float>> t_elooEmooP;
    edm::EDGetTokenT<std::vector<float>> t_elscEta;
}

#endif
