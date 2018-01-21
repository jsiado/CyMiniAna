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

objectSelection::objectSelection(edm::ParameterSet const& cfg) :
  // Electron
  m_applyIso(cfg.getParameter<bool>("el_applyIso")),
  m_el_pt(cfg.getParameter<double>("el_pt")),
  m_el_eta(cfg.getParameter<double>("el_eta")),
  m_el_ID(cfg.getParameter<std::string>("el_id")),
  m_el_ISO(cfg.getParameter<std::string>("el_iso")),
  // Muon
  m_mu_pt(cfg.getParameter<double>("mu_pt")),
  m_mu_eta(cfg.getParameter<double>("mu_eta")),
  m_mu_ID(cfg.getParameter<std::string>("mu_id")),
  //m_mu_ISO(cfg.getParameter<std::string>("mu_iso")),
  m_mu_IsoMin(cfg.getParameter<double>("mu_IsoMin")),
  m_mu_IsoMax(cfg.getParameter<double>("mu_IsoMax")),
  // Neutrino
  m_nu_pt(cfg.getParameter<double>("nu_pt")),
  m_nu_eta(cfg.getParameter<double>("nu_eta")),
  // Jets
  m_jet_pt(cfg.getParameter<double>("jet_pt")),
  m_jet_eta(cfg.getParameter<double>("jet_eta")),
  m_jet_ID(cfg.getParameter<std::string>("jet_id")),
  // Large-R Jets
  m_ljet_pt(cfg.getParameter<double>("ljet_pt")),
  m_ljet_eta(cfg.getParameter<double>("ljet_eta")),
  m_ljet_ID(cfg.getParameter<std::string>("ljet_id")){}



objectSelection::~objectSelection() {}


void objectSelection::initialize() {
    /* Initialize values for object selection */
    return;
}



bool objectSelection::pass( const Electron& el, bool isGen ) const{
    /* Cuts on electrons */
    bool pass(false);

    if (isGen){
        pass = true;
    }
    else{
        // kinematics
        bool passKin = (el.p4.Pt() > m_el_pt && std::abs(el.p4.Eta()) < m_el_eta);

        // id
        bool passID = electronID(el);

        pass = passKin && passID;
    }

    return pass;
}

bool objectSelection::pass( const Muon& mu, bool isGen ) const{
    /* Cuts on muons */
    bool pass(false);

    if (isGen){
        pass = true;
    }
    else{
        // kinematics
        bool passKin = (mu.p4.Pt() > m_mu_pt && std::abs(mu.p4.Eta()) < m_mu_eta);

        // id
        bool passID = muonID(mu);

        // iso
        bool passISO = muonISO(mu);

        pass = passKin && passID && passISO;
    }

    return pass;
}

bool objectSelection::pass( const Neutrino& nu, bool isGen ) const{
    /* Cuts on neutrinos */
    bool pass(false);

    if (isGen){
        pass = true;
    }
    else{
        pass = true;
        // quality cuts on neutrino reco? Possibly best for imaginary solutions!
    }

    return pass;
}

bool objectSelection::pass( const Jet& jet, bool isGen ) const{
    /* Cuts on jets */
    bool pass(false);

    if (isGen){
        pass = true;
    }
    else{
        // kinematics
        bool passKin = (jet.p4.Pt() > m_jet_pt && std::abs(jet.p4.Eta()) < m_jet_eta);

        // id
        bool passID = jetID(jet);

        pass = passKin && passID;
    }

    return pass;
}

bool objectSelection::pass( const Ljet& ljet, bool isGen ) const{
    /* Cuts on large-R jets */
    bool pass(false);

    if (isGen){
        pass = true;
    }
    else{
        // kinematics
        bool passKin = (ljet.p4.Pt() > m_ljet_pt || std::abs(ljet.p4.Eta()) < m_ljet_eta || ljet.subjets.size() >= m_ljet_nsubjets);

        // id
        bool passID = ljetID(ljet);

        pass = passKin && passID;
    }

    return pass;
}



/* ID AND ISOLATION FUNCTIONS */
bool objectSelection::applyElectronIsolation() const{
    /* Return electron isolation 
       This informs the 'Electrons' class on parameters
       needed for the ID calculation.
    */
    return m_applyIso;
}

bool objectSelection::electronID(const Electron& el) const{
    /* Electron ID */
    bool pass(false);

    if (m_el_ID.compare("veto")==0)
        pass = (el.vidVeto) ? true : false;
    else if (m_el_ID.compare("loose")==0)
        pass = (el.vidLoose) ? true : false;
    else if (m_el_ID.compare("medium")==0)
        pass = (el.vidMedium) ? true : false;
    else if (m_el_ID.compare("tight")==0)
        pass = (el.vidTight) ? true : false;
    else{
        edm::LogWarning("Unsupported electron ID type ") << m_el_ID;
        edm::LogWarning("Please check your electron ID configuration ");
        pass = false;
    }

    return pass;
}


bool objectSelection::muonID( const Muon& mu ) const{
    /* Muon ID */
    bool pass(false);

    if (m_mu_ID.compare("loose")==0 && mu.loose)
        pass = true;
    else if (m_mu_ID.compare("tight")==0 && mu.tight)
        pass = true;
    else{
        edm::LogWarning("Unsupported muon ID type ") << m_mu_ID;
        edm::LogWarning("Please check your muon ID configuration ");
        pass = false;
    }

    return pass;
}

bool objectSelection::muonISO( const Muon& mu ) const{
    /* Muon Isolation */
    bool pass(false);

    double muIso = mu.iso04;
    if (muIso >= m_mu_IsoMin && muIso < m_mu_IsoMax)
        pass = true;

    return pass;
}


bool objectSelection::jetID(const Jet& jet) const{
    /* Jet ID */
    bool pass(false);

    if (m_jet_ID.compare("loose")==0)
        pass = (jet.loose) ? true : false;
    else if (m_jet_ID.compare("tight")==0)
        pass = (jet.tight) ? true : false;
    else if (m_jet_ID.compare("tightlepveto")==0)
        pass = (jet.tightlepveto) ? true : false;
    else{
        edm::LogWarning("Unsupported jet ID type ");
        edm::LogWarning("ID = ") << m_jet_ID << m_jet_ID.compare("loose");
        edm::LogWarning("Please check your jet ID configuration ");
        pass = false;
    }

    return pass;
}


bool objectSelection::ljetID(const Ljet& ljet) const{
    /* Large-R jet ID */
    bool pass(false);

    if (m_ljet_ID.compare("loose")==0)
        pass = (ljet.loose) ? true : false;
    else if (m_ljet_ID.compare("tight")==0)
        pass = (ljet.tight) ? true : false;
    else if (m_ljet_ID.compare("tightlepveto")==0)
        pass = (ljet.tightlepveto) ? true : false;
    else{
        edm::LogWarning("Unsupported large-R jet ID type ") << m_ljet_ID;
        edm::LogWarning("Please check your ljet ID configuration ");
        pass = false;
    }

    return pass;
}

// THE END
