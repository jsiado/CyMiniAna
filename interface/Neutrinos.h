#ifndef NEUTRINOS_H
#define NEUTRINOS_H

#include <vector>
#include <iostream>

#include "TROOT.h"
#include "TLorentzVector.h"

#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "Analysis/CyMiniAna/interface/physicsObjects.h"
#include "Analysis/CyMiniAna/interface/objectSelection.h"

// Neutrinos Class
class Neutrinos {
  public:
    // Constructor
    explicit Neutrinos(edm::ParameterSet const& iConfig, edm::ConsumesCollector && iC);

    // Destructor
    virtual ~Neutrinos();

    // Container of Neutrinos (defined in interface/physicsObjects.h)
    std::vector<Neutrino> execute(const Lepton& lep, const MET& met, const objectSelection& obj);
    std::vector<Neutrino> execute(const edm::Event& evt, const objectSelection& obj);

  private:

    // Physics information
    std::vector<Neutrino> m_neutrinos;

    // Setup physics information from EDMntuples
    edm::ParameterSet m_labels;
    bool m_buildNeutrinos;

    // ************
    // Setup to read EDMntuple format
    // -- following this example:
    //    https://github.com/dmajumder/VLQAna
    // ----------member data ---------------------------
    edm::Handle<std::vector<float>> h_nu_pt;
    edm::Handle<std::vector<float>> h_nu_eta;
    edm::Handle<std::vector<float>> h_nu_phi;

    edm::EDGetTokenT<std::vector<float>> t_nu_pt;
    edm::EDGetTokenT<std::vector<float>> t_nu_eta;
    edm::EDGetTokenT<std::vector<float>> t_nu_phi;
};

#endif
