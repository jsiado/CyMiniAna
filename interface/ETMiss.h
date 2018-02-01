#ifndef ETMISS_H
#define ETMISS_H

#include <vector>
#include <iostream>

#include "TROOT.h"
#include "TLorentzVector.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "Analysis/CyMiniAna/interface/physicsObjects.h"
#include "Analysis/CyMiniAna/interface/objectSelection.h"

// ETMiss Class
class ETMiss {
  public:
    // Constructor
    explicit ETMiss(edm::ParameterSet const& iConfig, edm::ConsumesCollector && iC);

    // Destructor
    virtual ~ETMiss();

    // Container of MET (defined in interface/physicsObjects.h)
    MET execute(const edm::Event& evt, const objectSelection& obj);
    MET execute_truth(const edm::Event& evt, const objectSelection& obj);

  private:

    // Physics information
    MET m_MET;
    MET m_truth_MET;

    // Setup physics information from EDMntuples
    edm::ParameterSet m_labels;
    bool m_useTruth;


    // ************
    // Setup to read EDMntuple format
    // -- following this example:
    //    https://github.com/dmajumder/VLQAna
    // ----------member data ---------------------------
    // Handles
    // MET
    edm::Handle<std::vector<float>> h_metFullPhi;
    edm::Handle<std::vector<float>> h_metFullPt;
    edm::Handle<std::vector<float>> h_metFullPx;
    edm::Handle<std::vector<float>> h_metFullPy;
    edm::Handle<std::vector<float>> h_metFulluncorPhi;
    edm::Handle<std::vector<float>> h_metFulluncorPt;
    edm::Handle<std::vector<float>> h_metFulluncorSumEt;

    // Tokens
    edm::EDGetTokenT<std::vector<float>> t_metFullPhi;
    edm::EDGetTokenT<std::vector<float>> t_metFullPt;
    edm::EDGetTokenT<std::vector<float>> t_metFullPx;
    edm::EDGetTokenT<std::vector<float>> t_metFullPy;
    edm::EDGetTokenT<std::vector<float>> t_metFulluncorPhi;
    edm::EDGetTokenT<std::vector<float>> t_metFulluncorPt;
    edm::EDGetTokenT<std::vector<float>> t_metFulluncorSumEt;
    // Truth MET
};

#endif
