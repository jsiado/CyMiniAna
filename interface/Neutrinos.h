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


// Neutrinos Class
class Neutrinos {
  public:
    // Constructor
    explicit Neutrinos(edm::ParameterSet const& iConfig, edm::ConsumesCollector && iC);

    // Destructor
    virtual ~Neutrinos();

  private:

    // Container of Neutrinos (defined in interface/physicsObjects.h)
    std::vector<Neutrino> buildNeutrinos(const Lepton& lep, const MET& met);
    std::vector<Neutrino> execute(const edm::Event& evt);

    // Physics information
    std::vector<Neutrino> m_neutrinos;

    // Setup physics information from EDMntuples

    // ************
    // Setup to read EDMntuple format
    // -- following this example:
    //    https://github.com/dmajumder/VLQAna
    // ----------member data ---------------------------
    edm::Handle<std::vector<float>> h_nu_pt;
    edm::Handle<std::vector<float>> h_nu_eta;
    edm::Handle<std::vector<float>> h_nu_phi;
}

#endif
