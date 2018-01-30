/*
Created:        14 December 2017
Last Updated:   30 January  2018

Dan Marley
daniel.edison.marley@cernSPAMNOT.ch
Texas A&M University

-----

Write data to flat ntuple
*/
#include "Analysis/CyMiniAna/interface/EventSaverFlatNtuple.h"

using namespace edm;

EventSaverFlatNtuple::EventSaverFlatNtuple( const ParameterSet & cfg ) :
  t_electrons(consumes<std::vector<Electron>>(edm::InputTag("ana","electrons","CyMiniAna"))),
  t_muons(consumes<std::vector<Muon>>(edm::InputTag("ana","muons","CyMiniAna"))),
  t_neutrinos(consumes<std::vector<Neutrino>>(edm::InputTag("ana","neutrinos","CyMiniAna"))),
  t_jets(consumes<std::vector<Jet>>(edm::InputTag("ana","jets","CyMiniAna"))),
  t_ljets(consumes<std::vector<Ljet>>(edm::InputTag("ana","ljets","CyMiniAna"))),
  t_met(consumes<MET>(edm::InputTag("ana","MET","CyMiniAna"))),
  t_HT(consumes<double>(edm::InputTag("ana","HT","CyMiniAna"))),
  t_ST(consumes<double>(edm::InputTag("ana","ST","CyMiniAna"))){
    usesResource("TFileService");

    // options set by config
    m_isMC           = cfg.getParameter<bool>("isMC");           // filling truth branches
    m_useJets        = cfg.getParameter<bool>("useJets");        // filling jet branches
    m_useLargeRJets  = cfg.getParameter<bool>("useLargeRJets");  // filling large-R jet branches
    m_useLeptons     = cfg.getParameter<bool>("useLeptons");     // filling lepton branches
    m_useNeutrinos   = cfg.getParameter<bool>("useNeutrinos");   // filling neutrino branches 
}


EventSaverFlatNtuple::~EventSaverFlatNtuple() {}



void EventSaverFlatNtuple::beginJob(){
    /* Initialize TTree */
    m_ttree = m_fs->make<TTree>("events","events");
    std::cout << " EVENT SAVER BEGINJOB -- made m_ttree " << std::endl;

    initialize_branches();
    std::cout << " EVENT SAVER BEGINJOB -- init branches" << std::endl;
    return;
}


void EventSaverFlatNtuple::analyze( const edm::Event& event, const edm::EventSetup& ) {
//void EventSaverFlatNtuple::produce(edm::Event& event, const edm::EventSetup& ){
    /* Fill TTree 
       This is the function to modify / inherit for analysis-specific purposes
    */
    // load objects from the event
    event.getByToken( t_electrons, m_electrons );
    event.getByToken( t_muons, m_muons );
    event.getByToken( t_neutrinos, m_neutrinos );
    event.getByToken( t_jets, m_jets );
    event.getByToken( t_ljets, m_ljets );
    event.getByToken( t_met, m_met );
    event.getByToken( t_HT,  m_HT );
    event.getByToken( t_ST,  m_ST );


    // Set branch values


    // Fill output tree
    m_ttree->Fill();

    return;
}


void EventSaverFlatNtuple::initialize_branches(){
    /* Setup the output tree */
    // Physics Objects
    // -- AK4 Jets
    m_ttree->Branch("jet_pt",  &m_jet_pt);     // vector of floats
    m_ttree->Branch("jet_eta", &m_jet_eta);    // vector of floats
    m_ttree->Branch("jet_phi", &m_jet_phi);    // vector of floats
    m_ttree->Branch("jet_e",   &m_jet_e);      // vector of floats
    m_ttree->Branch("jet_CSV", &m_jet_CSV);    // vector of floats
    m_ttree->Branch("jet_charge",      &m_jet_charge);        // vector of floats
    m_ttree->Branch("jet_ID_loose",    &m_jet_ID_loose);      // vector of ints
    m_ttree->Branch("jet_ID_medium",   &m_jet_ID_medium);     // vector of ints
    m_ttree->Branch("jet_ID_tight",    &m_jet_ID_tight);      // vector of ints
    m_ttree->Branch("jet_ID_tightlepveto", &m_jet_ID_tightlepveto);  // vector of ints
    m_ttree->Branch("jet_true_flavor", &m_jet_true_flavor);   // vector of ints

    // -- AK8 Jets
    m_ttree->Branch("ljet_pt",  &m_ljet_pt);     // vector of floats
    m_ttree->Branch("ljet_eta", &m_ljet_eta);    // vector of floats
    m_ttree->Branch("ljet_phi", &m_ljet_phi);    // vector of floats
    m_ttree->Branch("ljet_e",   &m_ljet_e);      // vector of floats
    m_ttree->Branch("ljet_CSV", &m_ljet_CSV);    // vector of floats
    m_ttree->Branch("ljet_tau1",      &m_ljet_tau1);          // vector of floats
    m_ttree->Branch("ljet_tau2",      &m_ljet_tau2);          // vector of floats
    m_ttree->Branch("ljet_tau3",      &m_ljet_tau3);          // vector of floats
    m_ttree->Branch("ljet_tau21",     &m_ljet_tau21);         // vector of floats
    m_ttree->Branch("ljet_tau32",     &m_ljet_tau32);         // vector of floats
    m_ttree->Branch("ljet_charge",    &m_ljet_charge);        // vector of floats
    m_ttree->Branch("ljet_SDmass",    &m_ljet_SDmass);        // vector of floats
    m_ttree->Branch("ljet_ID_loose",  &m_ljet_ID_loose);      // vector of ints
    m_ttree->Branch("ljet_ID_medium", &m_ljet_ID_medium);     // vector of ints
    m_ttree->Branch("ljet_ID_tight",  &m_ljet_ID_tight);      // vector of ints
    m_ttree->Branch("ljet_ID_tightlepveto", &m_ljet_ID_tightlepveto);  // vector of ints
    m_ttree->Branch("ljet_subjetIndex0",    &m_ljet_subjetIndex0);     // vector of ints
    m_ttree->Branch("ljet_subjetIndex1",    &m_ljet_subjetIndex1);     // vector of ints

    m_ttree->Branch("ljet_subjet_pt",     &m_ljet_subjet_pt);     // vector of floats
    m_ttree->Branch("ljet_subjet_eta",    &m_ljet_subjet_eta);    // vector of floats
    m_ttree->Branch("ljet_subjet_phi",    &m_ljet_subjet_phi);    // vector of floats
    m_ttree->Branch("ljet_subjet_e",      &m_ljet_subjet_e);      // vector of floats
    m_ttree->Branch("ljet_subjet_CSV",    &m_ljet_subjet_CSV);    // vector of floats
    m_ttree->Branch("ljet_subjet_charge", &m_ljet_subjet_charge); // vector of floats

    // -- Leptons (electrons, muons, neutrinos)
    m_ttree->Branch("el_pt",  &m_el_pt);     // vector of floats
    m_ttree->Branch("el_eta", &m_el_eta);    // vector of floats
    m_ttree->Branch("el_phi", &m_el_phi);    // vector of floats
    m_ttree->Branch("el_e",   &m_el_e);      // vector of floats
    m_ttree->Branch("el_iso", &m_el_iso);    // vector of ints
    m_ttree->Branch("el_charge",    &m_el_charge);        // vector of floats
    m_ttree->Branch("el_ID_loose",  &m_el_ID_loose);      // vector of ints
    m_ttree->Branch("el_ID_medium", &m_el_ID_medium);     // vector of ints
    m_ttree->Branch("el_ID_tight",  &m_el_ID_tight);      // vector of ints

    m_ttree->Branch("mu_pt",  &m_mu_pt);     // vector of floats
    m_ttree->Branch("mu_eta", &m_mu_eta);    // vector of floats
    m_ttree->Branch("mu_phi", &m_mu_phi);    // vector of floats
    m_ttree->Branch("mu_e",   &m_mu_e);      // vector of floats
    m_ttree->Branch("mu_iso", &m_mu_iso);    // vector of ints
    m_ttree->Branch("mu_charge",    &m_mu_charge);        // vector of floats
    m_ttree->Branch("mu_ID_loose",  &m_mu_ID_loose);      // vector of ints
    m_ttree->Branch("mu_ID_medium", &m_mu_ID_medium);     // vector of ints
    m_ttree->Branch("mu_ID_tight",  &m_mu_ID_tight);      // vector of ints

    m_ttree->Branch("nu_pt",  &m_nu_pt);     // vector of floats
    m_ttree->Branch("nu_eta", &m_nu_eta);    // vector of floats
    m_ttree->Branch("nu_phi", &m_nu_phi);    // vector of floats
    m_ttree->Branch("nu_e",   &m_nu_e);      // vector of floats

    // MET, HT, ST
    m_ttree->Branch("met_met",  &m_met_met, "met_met/F"); // float
    m_ttree->Branch("met_phi",  &m_met_phi, "met_phi/F"); // float
    m_ttree->Branch("HT",       &m_HT,      "HT/F");      // float
    m_ttree->Branch("ST",       &m_ST,      "ST/F");      // float

    // Event information
    //    Don't save flags/triggers (already used in EDM ntuples/MiniAOD!)
    m_ttree->Branch("runNumber",       &m_runNumber,   "runNumber/I");      // int
    m_ttree->Branch("eventNumber",     &m_eventNumber, "eventNumber/I");    // int
    m_ttree->Branch("lumiblock",       &m_lumiblock,   "lumiblock/I");      // int
    m_ttree->Branch("rho",             &m_rho,         "rho/F");            // float
    m_ttree->Branch("nGoodVtx",        &m_nGoodVtx,    "nGoodVtx/I");       // int
    m_ttree->Branch("LHA_PDF_ID",      &m_LHA_PDF_ID,  "LHA_PDF_ID/I");     // int
    m_ttree->Branch("nIsoTrk",         &m_nIsoTrk,     "nIsoTrk/I");        // int
    m_ttree->Branch("true_pileup",     &m_true_pileup, "true_pileup/I");    // int

    // -- Weights
    m_ttree->Branch("weight_mc",       &m_weight_mc,      "weight_mc/F");          // float
    m_ttree->Branch("weight_btag",     &m_weight_btag,    "weight_btag/F");        // float
    m_ttree->Branch("weight_pileup",   &m_weight_pileup,  "weight_pileup/F");      // float
    m_ttree->Branch("weight_jet_jer",  &m_weight_jet_jer, "weight_jet_jer/F");     // float
    m_ttree->Branch("weight_;jet_jer", &m_weight_ljet_jer,"weight_ljet_jer/F");    // float
    m_ttree->Branch("xsection",        &m_xsection,       "xsection/F");           // float
    m_ttree->Branch("kfactor",         &m_kfactor,        "kfactor/F");            // float
    m_ttree->Branch("sumOfWeights",    &m_sumOfWeights,   "sumOfWeights/F");       // float

    // Uncertainties (SFs)
    m_ttree->Branch("met_met_sf",      &m_met_met_sf,    "met_met_sf/F");          // float
    m_ttree->Branch("met_phi_sf",      &m_met_phi_sf,    "met_phi_sf/F");          // float

    m_ttree->Branch("jet_jec",         &m_jet_jec);        // vector of floats
    m_ttree->Branch("jet_jer_up",      &m_jet_jer_up);     // vector of floats
    m_ttree->Branch("jet_jer_down",    &m_jet_jer_down);   // vector of floats

    m_ttree->Branch("ljet_jec",        &m_ljet_jec);       // vector of floats
    m_ttree->Branch("ljet_jer_up",     &m_ljet_jer_up);    // vector of floats
    m_ttree->Branch("ljet_jer_down",   &m_ljet_jer_down);  // vector of floats

    // btagging
    // leptons


    // Misc.
    // -- Generator-level information
    //    Save W',T/B,top,bottom,W/Z,Higgs,tau,el,mu,nu 
    //    and anything that comes from a top/W/Z/Higgs/tau decay
    m_ttree->Branch("mc_pt",        &m_mc_pt);       // vector of floats
    m_ttree->Branch("mc_eta",       &m_mc_eta);      // vector of floats
    m_ttree->Branch("mc_phi",       &m_mc_phi);      // vector of floats
    m_ttree->Branch("mc_e",         &m_mc_e);        // vector of floats
    m_ttree->Branch("mc_pdgId",     &m_mc_pdgId);    // vector of floats
    m_ttree->Branch("mc_charge",    &m_mc_charge);   // vector of floats
    m_ttree->Branch("mc_mom_idx",   &m_mc_mom_idx);  // vector of floats

    // -- Truth-level Objects (AK4/AK8 Jets)
    m_ttree->Branch("truth_jet_pt",      &m_truth_jet_pt);      // vector of floats
    m_ttree->Branch("truth_jet_eta",     &m_truth_jet_eta);     // vector of floats
    m_ttree->Branch("truth_jet_phi",     &m_truth_jet_phi);     // vector of floats
    m_ttree->Branch("truth_jet_e",       &m_truth_jet_e);       // vector of floats
    m_ttree->Branch("truth_jet_charge",  &m_truth_jet_charge);  // vector of floats

    m_ttree->Branch("truth_ljet_pt",     &m_truth_ljet_pt);     // vector of floats
    m_ttree->Branch("truth_ljet_eta",    &m_truth_ljet_eta);    // vector of floats
    m_ttree->Branch("truth_ljet_phi",    &m_truth_ljet_phi);    // vector of floats
    m_ttree->Branch("truth_ljet_e",      &m_truth_ljet_e);      // vector of floats
    m_ttree->Branch("truth_ljet_charge", &m_truth_ljet_charge); // vector of floats

    return;
}


DEFINE_FWK_MODULE(EventSaverFlatNtuple);
// THE END
