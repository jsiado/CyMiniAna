/*
Created:        --
Last Updated:   18 December 2017

Dan Marley
daniel.edison.marley@cernSPAMNOT.ch
Texas A&M University

-----

Build Neutrinos from EDMntuples
 -- Build using W mass constraint :: buildNeutrinos()
 -- Future ntuples may store neutrino pt, eta, phi :: execute()
*/
#include "Analysis/CyMiniAna/interface/Neutrinos.h"


using namespace edm; 


Neutrinos::Neutrinos(edm::ParameterSet const& iConfig, edm::ConsumesCollector && iC) : 
  m_buildNeutrinos(iConfig.getParameter<float>("buildNeutrinos")){
    if (!m_buildNeutrinos){
      t_nu_pt  = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("nuPtLabel"));
      t_nu_eta = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("nuEtaLabel"));
      t_nu_phi = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("nuPhiLabel"));
    }
}

Neutrinos::~Neutrinos() {}


std::vector<Neutrino> Neutrinos::execute(const Lepton& lep, const MET& met, const objectSelection& obj){
    /* Reconstruct neutrino using W mass constraint: MET+lepton */
    m_neutrinos.clear();
    return m_neutrinos;
}



std::vector<Neutrino> Neutrinos::execute(const edm::Event& evt, const objectSelection& obj){
    /* Build the Neutrinos from ntuple */
    m_neutrinos.clear();

    evt.getByToken(t_nu_pt,  h_nu_pt);
    evt.getByToken(t_nu_eta, h_nu_eta);
    evt.getByToken(t_nu_phi, h_nu_phi);

    for (unsigned int inu=0, size=(h_nu_pt.product())->size(); inu<size; ++inu) {
        Neutrino nu;

        float pt  = (h_nu_pt.product())->at(inu);
        float eta = (h_nu_eta.product())->at(inu);
        float phi = (h_nu_phi.product())->at(inu);

        nu.p4.SetPtEtaPhiM(pt,eta,phi,0.);

        m_neutrinos.push_back(nu);
    }

    return m_neutrinos;
}


// THE END