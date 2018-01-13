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
    setJetID();
    setLjetID();

    return;
}



bool objectSelection::pass( const Electron& el, bool isGen ) const{
    /* Cuts on electrons */
    bool pass(false);

    if (isGen){
        pass = true;
    }
    else{
        pass = true;
        // kinematics
        if (el.p4.Pt() < m_el_pt || std::abs(el.p4.Eta()) > m_el_eta)
            pass = false;

        // id
        if (m_el_ID.compare("veto")==0 && !el.vidVeto)
            pass = false;
        else if (m_el_ID.compare("loose")==0 && !el.vidLoose)
            pass = false;
        else if (m_el_ID.compare("medium")==0 && !el.vidMedium)
            pass = false;
        else if (m_el_ID.compare("tight")==0 && !el.vidTight)
            pass = false;

        // iso -- not applicable 11 Jan 2018 (looking for recommendations)
        // used in ID
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
        pass = true;
        // kinematics
        if (mu.p4.Pt() < m_mu_pt || std::abs(mu.p4.Eta()) > m_mu_eta)
            pass = false;
        // id
        if (!muonID(mu))
            pass = false;
        // iso
        if (!muonISO(mu))
            pass = false;
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
        pass = true;
        // kinematics
        if (jet.p4.Pt() < m_jet_pt || std::abs(jet.p4.Eta()) > m_jet_eta)
            pass = false;
        // id
        if (!jetID(jet))
            pass = false;
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
        pass = true;
        // kinematics
        if (ljet.p4.Pt() < m_ljet_pt || std::abs(ljet.p4.Eta()) > m_ljet_eta)
            pass = false;
        // id
        if (!ljetID(ljet))
            pass = false;
    }

    return pass;
}



/* ID AND ISOLATION FUNCTIONS */


void objectSelection::electronID( Electron& el ) const{
    /* Determine electron ID */
    int missHits = el.missHits;
    bool isEB = (std::abs(el.scEta) <= m_barrel_eta);
    bool conv = el.hasMatchedConVeto;
    float Dz  = std::abs(el.Dz);
    float Dxy = std::abs(el.Dxy);
    float HoE = el.HoE;
    float dPhiIn   = std::abs(el.dPhiIn);
    float ooEmooP  = el.ooEmooP;
    float dEtaInSeed  = std::abs(el.dEtaInSeed);
    float full5x5siee = el.full5x5siee;

    // All ID must pass this parameter!
    if (conv) {
        el.vidVeto   = false;
        el.vidLoose  = false;
        el.vidMedium = false;
        el.vidTight  = false;
        return;
    }

    // RelIsoEA requires special treatment in barrel vs endcap
    bool elISO_veto   = electronISO(el,"veto");
    bool elISO_loose  = electronISO(el,"loose");
    bool elISO_medium = electronISO(el,"medium");
    bool elISO_tight  = electronISO(el,"tight");

    // loop over different isolations to reduce code duplication
    // 12 Jan: Need smarter way to do this that is simpler and uses less CPU
    std::map<std::string,bool> isos = { {"veto",elISO_veto}, {"loose",elISO_loose}, 
                                        {"medium",elISO_medium}, {"tight",elISO_tight} };
    std::map<std::string,bool> ids  = { {"veto",false}, {"loose",false}, {"medium",false}, {"tight",false} };

    for (const auto& iso : isos){
        bool isVETO = (dEtaInSeed < m_dEtaInSeed.at(iso.first).at(isEB)) &&
                  (dPhiIn < m_dPhiIn.at(iso.first).at(isEB)) && 
                  (full5x5siee < m_full5x5siee.at(iso.first).at(isEB)) && 
                  (HoE < m_HoE.at(iso.first).at(isEB)) && 
                  (Dxy < m_Dxy.at(iso.first).at(isEB)) && 
                  (Dz  < m_Dz.at(iso.first).at(isEB))  && 
                  (ooEmooP < m_ooEmooP.at(iso.first).at(isEB)) && 
                  (missHits <= m_missHits.at(iso.first).at(isEB)) && 
                  (iso.second);
        ids.at(iso.first) = isVETO;
    }

    el.vidVeto   = ids.at("veto");
    el.vidLoose  = ids.at("loose");
    el.vidMedium = ids.at("medium");
    el.vidTight  = ids.at("tight");

    return;
}


bool objectSelection::electronISO( const Electron& el, const std::string& ID ) const{
    /* Electron Isolation */
    bool elISO(false);
    float RelIsoEA = el.RelIsoEA;

    if ( std::abs(el.scEta) <= m_barrel_eta )
        elISO = (RelIsoEA < m_RelIsoEA.at(ID).at(true));
    else
        elISO = m_applyIso ? (RelIsoEA < m_RelIsoEA.at(ID).at(false)) : true;

    return elISO;
}


bool objectSelection::muonID( const Muon& mu ) const{
    /* Muon ID */
    bool pass(false);

    if (m_mu_ID.compare("loose")==0 && mu.loose)
        pass = true;
    else if (m_mu_ID.compare("tight")==0 && mu.tight)
        pass = true;
    else
        pass = false;

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



void objectSelection::setJetID(){
    /* Set values for the jet ID */
    m_jetindexNEF = 0.9;
    m_jetIndexCHF = 0.0;
    m_jetindexNHF = 0.9;
    m_jetIndexCEF = 0.99;
    m_jetIndexNCH = 0;
    m_jetIndexMUE = 0.8;
    m_jetindexNConstituents = 1;

    if ( m_jet_ID == "loose" ) {
        m_jetindexNHF = 0.99;
        m_jetindexNEF = 0.99;
        m_jetIndexMUE = 0.0;
    }
    else if ( m_jet_ID == "tight" ) {
        m_jetIndexMUE = 0.0;
    }
    else if ( m_jet_ID == "tightlepveto" ) {
        m_jetIndexCEF = 0.9;
    }

    return;
}


bool objectSelection::jetID( const Jet& jet ) const{
    /* Jet ID */
    bool pass(false);

    int nconstituents = jet.cMultip+jet.nMultip;

    // Cuts for all |eta|:
    if ( nconstituents > m_jetindexNConstituents && jet.nEMEnergy < m_jetindexNEF &&
         jet.nHadEnergy < m_jetindexNHF ){
        pass = true;
    }

    // Cuts for |eta| < 2.4:
    if ( std::abs(jet.p4.Eta()) < 2.4 ){
        if ( jet.cEMEnergy < m_jetIndexCEF  && jet.cHadEnergy > m_jetIndexCHF &&
             jet.cMultip > m_jetIndexNCH    && jet.muonEnergy / jet.p4.E() < m_jetIndexMUE ){
            pass = true;
        }
        else{
            pass = false;
        }
    }

    return pass;
}


void objectSelection::setLjetID(){
    /* Set values for the jet ID */
    m_ljetindexNEF = 0.9;
    m_ljetIndexCHF = 0.0;
    m_ljetindexNHF = 0.9;
    m_ljetIndexCEF = 0.99;
    m_ljetIndexNCH = 0;
    m_ljetIndexMUE = 0.8;
    m_ljetindexNConstituents = 1;

    if ( m_ljet_ID == "loose" ) {
        m_ljetindexNHF = 0.99;
        m_ljetindexNEF = 0.99;
        m_ljetIndexMUE = 0.0;
    }
    else if ( m_ljet_ID == "tight" ) {
        m_ljetIndexMUE = 0.0;
    }
    else if ( m_ljet_ID == "tightlepveto" ) {
        m_ljetIndexCEF = 0.9;
    }

    return;
}

bool objectSelection::ljetID( const Ljet& ljet ) const{
    /* Large-R Jet ID */
    bool pass(false);

    int nconstituents = ljet.cMultip+ljet.nMultip;

    // Cuts for all |eta|:
    if ( nconstituents > m_jetindexNConstituents && ljet.nEMEnergy < m_jetindexNEF &&
         ljet.nHadEnergy < m_jetindexNHF ){
        pass = true;
    }

    // Cuts for |eta| < 2.4:
    if ( std::abs(ljet.p4.Eta()) < 2.4 ){
        if ( ljet.cEMEnergy < m_jetIndexCEF && ljet.cHadEnergy > m_jetIndexCHF &&
             ljet.cMultip > m_jetIndexNCH   && ljet.muonEnergy / ljet.p4.E() < m_jetIndexMUE ){
            pass = true;
        }
        else{
            pass = false;
        }
    }

    return pass;
}


// THE END
