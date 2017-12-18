#ifndef JETS_H
#define JETS_H

#include <vector>
#include <iostream>

#include "TROOT.h"
#include "TLorentzVector.h"

#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "Analysis/CyMiniAna/interface/physicsObjects.h"


// Jets Class
class Jets {
  public:
    // Constructor
    explicit Jets(edm::ParameterSet const& iConfig, edm::ConsumesCollector && iC);

    // Destructor
    virtual ~Jets();

  private:

    // Container of Jets (defined in interface/physicsObjects.h)
    std::vector<Jet> execute(const edm::Event& evt);
    std::vector<Jet> execute_truth(const edm::Event& evt);

    // Physics information
    std::vector<Jet> m_jets;
    std::vector<Jet> m_truth_jets;

    // Setup physics information from EDMntuples
    float m_jetPtMin;
    float m_jetAbsEtaMax;


    // ************
    // Setup to read EDMntuple format
    // -- following this example:
    //    https://github.com/dmajumder/VLQAna
    // ----------member data ---------------------------
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
}

#endif
