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
#include "Analysis/CyMiniAna/interface/objectSelection.h"
#include "Analysis/CyMiniAna/interface/BTagTools.h"

// Jets Class
class Jets {
  public:
    // Constructor
    explicit Jets(edm::ParameterSet const& iConfig, edm::ConsumesCollector && iC);

    // Destructor
    virtual ~Jets();

    // Container of Jets (defined in interface/physicsObjects.h)
    std::vector<Jet> execute(const edm::Event& evt, const objectSelection& obj);
    std::vector<Jet> execute_truth(const edm::Event& evt, const objectSelection& obj);

    void setJetID(Jet& jet) const;

  private:

    // Physics information
    std::vector<Jet> m_jets;
    std::vector<Jet> m_truth_jets;

    // Setup physics information from EDMntuples
    edm::ParameterSet m_labels;
    bool m_isMC;
    bool m_useTruth;
    std::string m_data_path;

    BTagTools *m_btagTool;

    // ************
    // Setup to read EDMntuple format
    // -- following this example:
    //    https://github.com/dmajumder/VLQAna
    // ----------member data ---------------------------
    edm::Handle<int> h_npv;
    edm::Handle<float> h_rho;
    edm::Handle<std::vector<float>> h_jetPt;
    edm::Handle<std::vector<float>> h_jetEta;
    edm::Handle<std::vector<float>> h_jetPhi;
    edm::Handle<std::vector<float>> h_jetE;
    edm::Handle<std::vector<float>> h_jetHadronFlavour;
    edm::Handle<std::vector<float>> h_jetPartonFlavour;
    edm::Handle<std::vector<float>> h_jetCSV;
    edm::Handle<std::vector<float>> h_jetCMVA;
    edm::Handle<std::vector<float>> h_jetCvsB;
    edm::Handle<std::vector<float>> h_jetCvsL;
    edm::Handle<std::vector<float>> h_jetJEC;
    edm::Handle<std::vector<float>> h_jetJERSF;
    edm::Handle<std::vector<float>> h_jetJERSFUp;
    edm::Handle<std::vector<float>> h_jetJERSFDown;
    edm::Handle<std::vector<float>> h_jetnHadEnergy;
    edm::Handle<std::vector<float>> h_jetnEMEnergy;
    edm::Handle<std::vector<float>> h_jetcHadEnergy;
    edm::Handle<std::vector<float>> h_jetcEMEnergy;
    edm::Handle<std::vector<float>> h_jetcMultip;
    edm::Handle<std::vector<float>> h_jetnMultip;
    edm::Handle<std::vector<float>> h_jetY;
    edm::Handle<std::vector<float>> h_jetArea;
    edm::Handle<std::vector<float>> h_jetMuonEnergy;
    // Truth jets
    edm::Handle<std::vector<float>> h_jetGenE;
    edm::Handle<std::vector<float>> h_jetGenEta;
    edm::Handle<std::vector<float>> h_jetGenPt;
    edm::Handle<std::vector<float>> h_jetGenPhi;
    edm::Handle<std::vector<float>> h_jetGenCharge;

    // Tokens
    edm::EDGetTokenT<int> t_npv;
    edm::EDGetTokenT<float> t_rho;
    edm::EDGetTokenT<std::vector<float>> t_jetPt;
    edm::EDGetTokenT<std::vector<float>> t_jetEta;
    edm::EDGetTokenT<std::vector<float>> t_jetPhi;
    edm::EDGetTokenT<std::vector<float>> t_jetE;
    edm::EDGetTokenT<std::vector<float>> t_jetHadronFlavour;
    edm::EDGetTokenT<std::vector<float>> t_jetPartonFlavour;
    edm::EDGetTokenT<std::vector<float>> t_jetCSV;
    edm::EDGetTokenT<std::vector<float>> t_jetCMVA;
    edm::EDGetTokenT<std::vector<float>> t_jetCvsB;
    edm::EDGetTokenT<std::vector<float>> t_jetCvsL;
    edm::EDGetTokenT<std::vector<float>> t_jetJEC;
    edm::EDGetTokenT<std::vector<float>> t_jetJERSF;
    edm::EDGetTokenT<std::vector<float>> t_jetJERSFUp;
    edm::EDGetTokenT<std::vector<float>> t_jetJERSFDown;
    edm::EDGetTokenT<std::vector<float>> t_jetnHadEnergy;
    edm::EDGetTokenT<std::vector<float>> t_jetnEMEnergy;
    edm::EDGetTokenT<std::vector<float>> t_jetcHadEnergy;
    edm::EDGetTokenT<std::vector<float>> t_jetcEMEnergy;
    edm::EDGetTokenT<std::vector<float>> t_jetcMultip;
    edm::EDGetTokenT<std::vector<float>> t_jetnMultip;
    edm::EDGetTokenT<std::vector<float>> t_jetY;
    edm::EDGetTokenT<std::vector<float>> t_jetArea;
    edm::EDGetTokenT<std::vector<float>> t_jetMuonEnergy;
    // Truth jets
    edm::EDGetTokenT<std::vector<float>> t_jetGenE;
    edm::EDGetTokenT<std::vector<float>> t_jetGenEta;
    edm::EDGetTokenT<std::vector<float>> t_jetGenPt;
    edm::EDGetTokenT<std::vector<float>> t_jetGenPhi;
    edm::EDGetTokenT<std::vector<float>> t_jetGenCharge;
};

#endif
