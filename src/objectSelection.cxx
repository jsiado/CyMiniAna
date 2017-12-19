/*
Created:        18 December 2017
Last Updated:   18 December 2017

Dan Marley
daniel.edison.marley@cernSPAMNOT.ch
Texas A&M University

-----

Object selection
*/
#include "Analysis/CyMiniAna/interface/objectSelection.h"

using namespace edm;

objectSelection::objectSelection() {}

~objectSelection::objectSelection() {}


void objectSelection::initialize(const ParameterSet & cfg) {
    /* Initialize values for object selection */
    // Electron
    m_el_pt  = cfg.;
    m_el_eta = cfg.;
    m_el_ID  = cfg.;
    m_el_ISO = cfg.;

    // Muon
    m_mu_pt  = cfg.;
    m_mu_eta = cfg.;
    m_mu_ID  = cfg.;
    m_mu_ISO = cfg.;

    // Neutrino
    m_nu_pt  = cfg.;
    m_nu_eta = cfg.;

    // Jets
    m_jet_pt  = cfg.;
    m_jet_eta = cfg.;
    m_jet_ID  = cfg.;
    m_jet_ISO = cfg.; // quality

    // Large-R Jets
    m_ljet_pt  = cfg.;
    m_ljet_eta = cfg.;
    m_ljet_ID  = cfg.; // tagging, substructure

    return;
}


bool objectSelection::pass( const CmaBase& ob ){
    /* Cuts on kinematics */
    bool pass(false);
    return pass;
}

bool objectSelection::pass( const Electron& el, bool isGen ){
    /* Cuts on electrons */
    bool pass(false);

    if (isGen){
    }
    else{
    }

    return pass;
}

bool objectSelection::pass( const Muon& mu, bool isGen ){
    /* Cuts on muons */
    bool pass(false);

    if (isGen){
    }
    else{
    }

    return pass;
}

bool objectSelection::pass( const Neutrino& nu, bool isGen ){
    /* Cuts on neutrinos */
    bool pass(false);

    if (isGen){
    }
    else{
    }

    return pass;
}

bool objectSelection::pass( const Jet& jet, bool isGen ){
    /* Cuts on jets */
    bool pass(false);

    if (isGen){
    }
    else{
    }

    return pass;
}

bool objectSelection::pass( const Ljet& ljet, bool isGen ){
    /* Cuts on large-R jets */
    bool pass(false);

    if (isGen){
    }
    else{
    }

    return pass;
}


// THE END