#ifndef LARGERJETS_H
#define LARGERJETS_H

#include <vector>
#include <iostream>

#include "TROOT.h"
#include "TLorentzVector.h"

#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "Analysis/CyMiniAna/interface/physicsObjects.h"


// LargeRJets Class
class LargeRJets {
  public:
    // Constructor
    explicit LargeRJets(edm::ParameterSet const& iConfig, edm::ConsumesCollector && iC);

    // Destructor
    virtual ~LargeRJets();

  private:

    // Container of LargeRJets (defined in interface/physicsObjects.h)
    std::vector<Ljet> execute(const edm::Event& evt);
    std::vector<Ljet> execute_truth(const edm::Event& evt);

    // Physics information
    std::vector<Ljet> m_ljets;
    std::vector<Ljet> m_truth_ljets;

    // Setup physics information from EDMntuples
    float m_ljetPtMin;
    float m_ljetAbsEtaMax;

    // ************
    // Setup to read EDMntuple format
    // -- following this example:
    //    https://github.com/dmajumder/VLQAna
    // ----------member data ---------------------------
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
}

#endif
