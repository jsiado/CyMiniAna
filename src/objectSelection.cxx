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

objectSelection::objectSelection(const ParameterSet& cfg) :
  m_config(&cfg){}


~objectSelection::objectSelection() {}


void objectSelection::initialize() {
    /* Initialize values for object selection */
    // Electron
    m_el_pt  = m_config->getParameter<float>("el_pt");
    m_el_eta = m_config->getParameter<float>("el_eta");
    m_el_ID  = m_lep_id_map[ m_config->getParameter<std::string>("el_id") ];
    m_el_ISO = m_lep_iso_map[ m_config->getParameter<std::string>("el_iso") ];
    m_applyIso = m_config->getParameter<bool>("el_applyIso");

    // Muon
    m_mu_pt  = m_config->getParameter<float>("mu_pt");
    m_mu_eta = m_config->getParameter<float>("mu_eta");
    m_mu_ID  = m_lep_id_map[ m_config->getParameter<std::string>("mu_id") ];
    //m_mu_ISO = m_lep_iso_map[ m_config->getParameter<std::string>("mu_iso") ];
    m_mu_IsoMin = m_config->getParameter<float>("mu_IsoMin");
    m_mu_IsoMax = m_config->getParameter<float>("mu_IsoMax");

    // Neutrino
    m_nu_pt  = m_config->getParameter<float>("nu_pt");
    m_nu_eta = m_config->getParameter<float>("nu_eta");

    // Jets
    m_jet_pt  = m_config->getParameter<float>("jet_pt");
    m_jet_eta = m_config->getParameter<float>("jet_eta");
    m_jet_ID  = m_lep_id_map[ m_config->getParameter<std::string>("jet_id") ];
    setJetID();

    // Large-R Jets
    m_ljet_pt  = m_config->getParameter<float>("ljet_pt");
    m_ljet_eta = m_config->getParameter<float>("ljet_eta");
    m_ljet_ID  = m_lep_id_map[ m_config->getParameter<std::string>("ljet_id") ]; // tagging, substructure
    setLjetID();

    return;
}



bool objectSelection::pass( const Electron& el, bool isGen ){
    /* Cuts on electrons */
    bool pass(false);

    if (isGen){
        pass = true;
    }
    else{
        pass = true;
        // kinematics
        if (el.p4.pt() < m_el_pt || std::abs(el.p4.Eta()) > m_el_eta)
            pass = false;

        // id
        bool updateElectronID(false);
        if (updateElectronID) electronID(el); // need to re-calculate the ID

        if (m_el_ID == lep_id::VETO && !el.vidVeto)
            pass = false;
        else if (m_el_ID == lep_id::LOOSE && !el.vidLoose)
            pass = false;
        else if (m_el_ID == lep_id::MEDIUM && !el.vidMedium)
            pass = false;
        else if (m_el_ID == lep_id::TIGHT && !el.vidTight)
            pass = false;

        // iso -- not applicable 11 Jan 2018 (looking for recommendations)
        // used in ID
    }

    return pass;
}

bool objectSelection::pass( const Muon& mu, bool isGen ){
    /* Cuts on muons */
    bool pass(false);

    if (isGen){
        pass = true;
    }
    else{
        pass = true;
        // kinematics
        if (mu.p4.pt() < m_mu_pt || std::abs(mu.p4.Eta()) > m_mu_eta)
            pass = false;
        // id
        if (muonID( mu ) != m_mu_ID)
            pass = false;
        // iso
        if (muonISO( mu ) != m_mu_ISO)
            pass = false;
    }

    return pass;
}

bool objectSelection::pass( const Neutrino& nu, bool isGen ){
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

bool objectSelection::pass( const Jet& jet, bool isGen ){
    /* Cuts on jets */
    bool pass(false);

    if (isGen){
        pass = true;
    }
    else{
        pass = true;
        // kinematics
        if (jet.p4.pt() < m_jet_pt || std::abs(jet.p4.Eta()) > m_jet_eta)
            pass = false;
        // id
        if (jetID( jet ) != m_jet_ID)
            pass = false;
    }

    return pass;
}

bool objectSelection::pass( const Ljet& ljet, bool isGen ){
    /* Cuts on large-R jets */
    bool pass(false);

    if (isGen){
        pass = true;
    }
    else{
        pass = true;
        // kinematics
        if (ljet.p4.pt() < m_ljet_pt || std::abs(ljet.p4.Eta()) > m_ljet_eta)
            pass = false;
        // id
        if (ljetID( ljet ) != m_ljet_ID)
            pass = false;
    }

    return pass;
}



/* ID AND ISOLATION FUNCTIONS */


void objectSelection::electronID( Electron& el ){
    /* Determine electron ID */
    bool pass(false);

    int missHits = el.missHits;
    bool isEB = (std::abs(el.scEta) <= m_barrel_eta);
    bool conv = el.hasMatchedConVeto;
    float Dz  = std::abs(el.Dz);
    float Dxy = std::abs(el.Dxy);
    float HoE = el.HoE;
    float dPhiIn   = std::abs(el.dPhiIn);
    float ooEmooP  = el.ooEmooP;
    float RelIsoEA = el.RelIsoEA;
    float dEtaInSeed  = std::abs(el.dEtaInSeed);
    float full5x5siee = el.full5x5siee;

    // All ID must pass this parameter!
    if (conv) {
        el.vidVETO   = false;
        el.vidLoose  = false;
        el.vidMedium = false;
        el.vidTight  = false;
        return;
    }

    bool elISO_veto   = electronISO(el,"veto");
    bool elISO_loose  = electronISO(el,"loose");
    bool elISO_medium = electronISO(el,"medium");
    bool elISO_tight  = electronISO(el,"tight");

    // RelIsoEA requires special treatment in barrel vs endcap
    bool isVETO = (dEtaInSeed < m_dEtaInSeed[lep_id::VETO][isEB]) &&
                  (dPhiIn < m_dPhiIn[lep_id::VETO][isEB]) && 
                  (full5x5siee < m_full5x5siee[lep_id::VETO][isEB]) && 
                  (HoE < m_HoE[lep_id::VETO][isEB]) && 
                  (Dxy < m_Dxy[lep_id::VETO][isEB]) && 
                  (Dz  < m_Dz[lep_id::VETO][isEB])  && 
                  (ooEmooP < m_ooEmooP[lep_id::VETO][isEB]) && 
                  (missHits <= m_missHits[lep_id::VETO][isEB]) && 
                  (elISO_veto);
    el.vidVeto = isVETO;


    // calculate loose first, then medium,tight
    bool isMEDIUM(false);
    bool isTIGHT(false);
    bool isLOOSE = (dEtaInSeed < m_dEtaInSeed[lep_id::LOOSE][isEB]) &&
                   (dPhiIn < m_dPhiIn[lep_id::LOOSE][isEB]) && 
                   (full5x5siee < m_full5x5siee[lep_id::LOOSE][isEB]) && 
                   (HoE < m_HoE[lep_id::LOOSE][isEB]) && 
                   (Dxy < m_Dxy[lep_id::LOOSE][isEB]) && 
                   (Dz  < m_Dz[lep_id::LOOSE][isEB])  && 
                   (ooEmooP < m_ooEmooP[lep_id::LOOSE][isEB]) && 
                   (missHits <= m_missHits[lep_id::LOOSE][isEB]) && 
                   (elISO_loose);

    if (isLOOSE) {
        // medium is a subset of loose
        // RelIsoEA requires special treatment in barrel vs endcap
        isMEDIUM =  (dEtaInSeed < m_dEtaInSeed[lep_id::MEDIUM][isEB]) &&
                    (dPhiIn < m_dPhiIn[lep_id::MEDIUM][isEB]) && 
                    (full5x5siee < m_full5x5siee[lep_id::MEDIUM][isEB]) && 
                    (HoE < m_HoE[lep_id::MEDIUM][isEB]) && 
                    (Dxy < m_Dxy[lep_id::MEDIUM][isEB]) && 
                    (Dz  < m_Dz[lep_id::MEDIUM][isEB])  && 
                    (ooEmooP < m_ooEmooP[lep_id::MEDIUM][isEB]) && 
                    (missHits <= m_missHits[lep_id::MEDIUM][isEB]) && 
                    (elISO_medium);
        if (isMEDIUM) {
            // tight is a subset of medium
            // RelIsoEA requires special treatment in barrel vs endcap
            isTIGHT =  (dEtaInSeed < m_dEtaInSeed[lep_id::TIGHT][isEB]) &&
                       (dPhiIn < m_dPhiIn[lep_id::TIGHT][isEB]) && 
                       (full5x5siee < m_full5x5siee[lep_id::TIGHT][isEB]) && 
                       (HoE < m_HoE[lep_id::TIGHT][isEB]) && 
                       (Dxy < m_Dxy[lep_id::TIGHT][isEB]) && 
                       (Dz  < m_Dz[lep_id::TIGHT][isEB])  && 
                       (ooEmooP < m_ooEmooP[lep_id::TIGHT][isEB]) && 
                       (missHits <= m_missHits[lep_id::TIGHT][isEB]) && 
                       (elISO_tight);
        }
    }
    
    el.vidLoose  = isLOOSE;
    el.vidMedium = isMEDIUM;
    el.vidTight  = isTIGHT;

    return;
}


bool objectSelection::electronISO( const Electron& el, const std::string& LEVEL ){
    /* Electron Isolation */
    bool elISO(false);
    lep_id id = m_lep_id_map[ LEVEL ];
    float RelIsoEA = el.RelIsoEA;

    if ( el.elscAbsEta <= m_barrel_eta )
        elISO = (RelIsoEA < m_RelIsoEA[id][true]);
    else
        elISO = m_applyIso ? (RelIsoEA < m_RelIsoEA[id][false]) : true;

    return elISO;
}


bool objectSelection::muonID( const Muon& mu ){
    /* Muon ID */
    lep_id id;

    bool pass(false);

    if (m_mu_ID == lep_id::LOOSE && mu.isLoose)
        pass = true;
    else if (m_mu_ID == lep_id::TIGHT && mu.isTight)
        pass = true;
    else
        pass = false;

    return pass;
}

bool objectSelection::muonISO( const Muon& mu ){
    /* Muon Isolation */
    bool pass(false);

    double muIso = mu.Iso04;
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

    if ( m_jet_ID == jet_id::LOOSE ) {
        m_jetindexNHF = 0.99;
        m_jetindexNEF = 0.99;
        m_jetIndexMUE = 0.0;
    }
    else if ( m_jet_ID == jet_id::TIGHT ) {
        m_jetIndexMUE = 0.0;
    }
    else if ( m_jet_ID == jet_id::TIGHTLEPVETO ) {
        m_jetIndexCEF = 0.9;
    }

    return;
}


bool objectSelection::jetID( const Jet& jet ){
    /* Jet ID */
    bool pass(false);

    int nconstituents = jet.cMultip+jet.nMultip;

    // Cuts for all |eta|:
    if ( nconstituents > m_jetindexNConstituents && jet.nEMEnergy < m_jetindexNEF &&
         jet.nHadEnergy < m_jetindexNHF ){
        pass = true;

    // Cuts for |eta| < 2.4:
    if ( std::abs(jet.p4.Eta()) < 2.4 ){
        if ( jet.cEMEnergy < m_jetIndexCEF  && jet.cHadEnergy > m_jetIndexCHF &&
             jet.cMultip > m_jetIndexNCH    && jet.MuonEnergy / jet.p4.E() < m_jetIndexMUE ){
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

    if ( m_ljet_ID == jet_id::LOOSE ) {
        m_ljetindexNHF = 0.99;
        m_ljetindexNEF = 0.99;
        m_ljetIndexMUE = 0.0;
    }
    else if ( m_ljet_ID == jet_id::TIGHT ) {
        m_ljetIndexMUE = 0.0;
    }
    else if ( m_ljet_ID == jet_id::TIGHTLEPVETO ) {
        m_ljetIndexCEF = 0.9;
    }

    return;
}

jet_id objectSelection::ljetID( const Ljet& ljet ){
    /* Large-R Jet ID */
    bool pass(false);

    int nconstituents = ljet.cMultip+ljet.nMultip;

    // Cuts for all |eta|:
    if ( nconstituents > m_jetindexNConstituents && ljet.nEMEnergy < m_jetindexNEF &&
         ljet.nHadEnergy < m_jetindexNHF ){
        pass = true;
    }

    // Cuts for |eta| < 2.4:
    if ( std::abs(jet.p4.Eta()) < 2.4 ){
        if ( ljet.cEMEnergy < m_jetIndexCEF && ljet.cHadEnergy > m_jetIndexCHF &&
             ljet.cMultip > m_jetIndexNCH   && ljet.MuonEnergy / ljet.p4.E() < m_jetIndexMUE ){
            pass = true;
        }
        else{
            pass = false;
        }
    }

    return id;
}


// THE END