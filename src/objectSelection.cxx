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
    bool passSel(false);

    if (isGen){
        passSel = true;
    }
    else{
        // kinematics
        bool passKin = (el.p4.Pt() > m_el_pt && std::abs(el.p4.Eta()) < m_el_eta);

        // id
        bool passID = electronID(el);

        passSel = passKin && passID;
    }

    return passSel;
}

bool objectSelection::pass( const Muon& mu, bool isGen ) const{
    /* Cuts on muons */
    bool passSel(false);

    if (isGen){
        passSel = true;
    }
    else{
        // kinematics
        bool passKin = (mu.p4.Pt() > m_mu_pt && std::abs(mu.p4.Eta()) < m_mu_eta);

        // id
        bool passID = muonID(mu);

        // iso
        bool passISO = muonISO(mu);

        passSel = passKin && passID && passISO;
    }

    return passSel;
}

bool objectSelection::pass( const Neutrino& nu, bool isGen ) const{
    /* Cuts on neutrinos */
    bool passSel(false);

    if (isGen){
        passSel = true;
    }
    else{
        passSel = true;
        // quality cuts on neutrino reco? Possibly best for imaginary solutions!
    }

    return passSel;
}

bool objectSelection::pass( const Jet& jet, bool isGen ) const{
    /* Cuts on jets */
    bool passSel(false);

    if (isGen){
        passSel = (jet.p4.Pt()>10 && std::abs(jet.p4.Eta()<5));
    }
    else{
        // kinematics
        bool passKin = (jet.p4.Pt() > m_jet_pt && std::abs(jet.p4.Eta()) < m_jet_eta);

        // id
        bool passID = jetID(jet);

        passSel = passKin && passID;
    }

    return passSel;
}

bool objectSelection::pass( const Ljet& ljet, bool isGen ) const{
    /* Cuts on large-R jets */
    bool passSel(false);

    if (isGen){
        passSel = (ljet.p4.Pt()>10 && std::abs(ljet.p4.Eta()<5));
    }
    else{
        // kinematics
        bool passKin = (ljet.p4.Pt() > m_ljet_pt || std::abs(ljet.p4.Eta()) < m_ljet_eta || ljet.subjets.size() >= m_ljet_nsubjets);

        // id
        bool passID = ljetID(ljet);

        passSel = passKin && passID;
    }

    return passSel;
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
    bool passSel(false);

    if (m_el_ID.compare("veto")==0)
        passSel = (el.vidVeto) ? true : false;
    else if (m_el_ID.compare("loose")==0)
        passSel = (el.vidLoose) ? true : false;
    else if (m_el_ID.compare("medium")==0)
        passSel = (el.vidMedium) ? true : false;
    else if (m_el_ID.compare("tight")==0)
        passSel = (el.vidTight) ? true : false;
    else{
        edm::LogWarning("Unsupported electron ID type ") << m_el_ID;
        edm::LogWarning("Please check your electron ID configuration ");
        passSel = false;
    }

    return passSel;
}


bool objectSelection::muonID( const Muon& mu ) const{
    /* Muon ID */
    bool passSel(false);

    if (m_mu_ID.compare("loose")==0 && mu.loose)
        passSel = true;
    else if (m_mu_ID.compare("tight")==0 && mu.tight)
        passSel = true;
    else if(m_mu_ID.compare("loose")!=0 && m_mu_ID.compare("tight")!=0){
        edm::LogWarning("Unsupported muon ID type ") << m_mu_ID;
        edm::LogWarning("Please check your muon ID configuration ");
        passSel = false;
    }

    return passSel;
}

bool objectSelection::muonISO( const Muon& mu ) const{
    /* Muon Isolation */
    bool passSel(false);

    double muIso = mu.iso04;
    if (muIso >= m_mu_IsoMin && muIso < m_mu_IsoMax)
        passSel = true;

    return passSel;
}


bool objectSelection::jetID(const Jet& jet) const{
    /* Jet ID */
    bool passSel(false);

    if (m_jet_ID.compare("loose")==0)
        passSel = (jet.loose) ? true : false;
    else if (m_jet_ID.compare("tight")==0)
        passSel = (jet.tight) ? true : false;
    else if (m_jet_ID.compare("tightlepveto")==0)
        passSel = (jet.tightlepveto) ? true : false;
    else{
        edm::LogWarning("Unsupported jet ID type ");
        edm::LogWarning("ID = ") << m_jet_ID << m_jet_ID.compare("loose");
        edm::LogWarning("Please check your jet ID configuration ");
        passSel = false;
    }

    return passSel;
}


bool objectSelection::ljetID(const Ljet& ljet) const{
    /* Large-R jet ID */
    bool passSel(false);

    if (m_ljet_ID.compare("loose")==0)
        passSel = (ljet.loose) ? true : false;
    else if (m_ljet_ID.compare("tight")==0)
        passSel = (ljet.tight) ? true : false;
    else if (m_ljet_ID.compare("tightlepveto")==0)
        passSel = (ljet.tightlepveto) ? true : false;
    else{
        edm::LogWarning("Unsupported large-R jet ID type ") << m_ljet_ID;
        edm::LogWarning("Please check your ljet ID configuration ");
        passSel = false;
    }

    return passSel;
}

// THE END
