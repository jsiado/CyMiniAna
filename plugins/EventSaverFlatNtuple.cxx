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
  t_electrons(consumes<std::vector<Electron>>(edm::InputTag("CMAProducer","electrons","CyMiniAna"))),
  t_muons(consumes<std::vector<Muon>>(edm::InputTag("CMAProducer","muons","CyMiniAna"))),
  t_neutrinos(consumes<std::vector<Neutrino>>(edm::InputTag("CMAProducer","neutrinos","CyMiniAna"))),
  t_jets(consumes<std::vector<Jet>>(edm::InputTag("CMAProducer","jets","CyMiniAna"))),
  t_ljets(consumes<std::vector<Ljet>>(edm::InputTag("CMAProducer","ljets","CyMiniAna"))),
  t_met(consumes<MET>(edm::InputTag("CMAProducer","MET","CyMiniAna"))),
  t_HT(consumes<double>(edm::InputTag("CMAProducer","HT","CyMiniAna"))),
  t_ST(consumes<double>(edm::InputTag("CMAProducer","ST","CyMiniAna"))),
  t_rho(consumes<std::vector<float>>(cfg.getParameter<edm::InputTag>("rhoLabel"))),
  t_npv(consumes<int>(cfg.getParameter<edm::InputTag>("npvLabel"))),
  t_sampleName(consumes<double>(edm::InputTag("CMAProducer","sampleName","CyMiniAna"))),
  t_xsection(consumes<double>(edm::InputTag("CMAProducer","xsection","CyMiniAna"))),
  t_kfactor(consumes<double>(edm::InputTag("CMAProducer","kfactor","CyMiniAna"))),
  t_sumOfWeights(consumes<double>(edm::InputTag("CMAProducer","sumOfWeights","CyMiniAna"))),
  t_LUMI(consumes<double>(cfg.getParameter<edm::InputTag>("LUMI"))){
    // Make output TTrees
    edm::Service<TFileService> fs;
    m_ttree = fs->make<TTree>("events","events");
    m_metadata_ttree = fs->make<TTree>("metadata","metadata");

    initialize_branches();

    // options set by config
    m_isMC           = cfg.getParameter<bool>("isMC");           // filling truth branches
    m_useJets        = cfg.getParameter<bool>("useJets");        // filling jet branches
    m_useLargeRJets  = cfg.getParameter<bool>("useLargeRJets");  // filling large-R jet branches
    m_useLeptons     = cfg.getParameter<bool>("useLeptons");     // filling lepton branches
    m_useNeutrinos   = cfg.getParameter<bool>("useNeutrinos");   // filling neutrino branches 
}


EventSaverFlatNtuple::~EventSaverFlatNtuple() {}



void EventSaverFlatNtuple::beginJob(){
    /* Begin Job */
    return;
}


void EventSaverFlatNtuple::analyze( const edm::Event& event, const edm::EventSetup& ) {
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
    event.getByToken( t_rho, h_rho);
    event.getByToken( t_npv, h_npv);
//    event.getByToken( t_runno,   h_runno);
//    event.getByToken( t_evtno,   h_evtno);
//    event.getByToken( t_lumisec, h_lumisec);

    // Set branch values
    if (m_useJets){
        m_jet_pt.clear();
        m_jet_eta.clear();
        m_jet_phi.clear();
        m_jet_e.clear();
        m_jet_CSV.clear();
        m_jet_charge.clear();
        m_jet_ID_loose.clear();
        m_jet_ID_medium.clear();
        m_jet_ID_tight.clear();
        m_jet_ID_tightlepveto.clear();
        m_jet_jec.clear();
        m_jet_jer_up.clear();
        m_jet_jer_down.clear();

        for (const auto& jet : *m_jets.product()){
            m_jet_pt.push_back(  jet.p4.Pt() );
            m_jet_eta.push_back( jet.p4.Eta() );
            m_jet_phi.push_back( jet.p4.Phi() );
            m_jet_e.push_back(   jet.p4.E() );
            m_jet_CSV.push_back( jet.CSVv2 );
            m_jet_charge.push_back(    jet.charge );
            m_jet_ID_loose.push_back(  jet.loose );
            m_jet_ID_medium.push_back( jet.medium );
            m_jet_ID_tight.push_back(  jet.tight );
            m_jet_ID_tightlepveto.push_back( jet.tightlepveto );
            m_jet_jec.push_back(1);
            m_jet_jer_up.push_back(1);
            m_jet_jer_down.push_back(1);
        }
    }
    if (m_useLargeRJets){
        m_ljet_pt.clear();
        m_ljet_eta.clear();
        m_ljet_phi.clear();
        m_ljet_e.clear();
        m_ljet_CSV.clear();
        m_ljet_tau1.clear();
        m_ljet_tau2.clear();
        m_ljet_tau3.clear();
        m_ljet_tau21.clear();
        m_ljet_tau32.clear();
        m_ljet_charge.clear();
        m_ljet_SDmass.clear();
        m_ljet_ID_loose.clear();
        m_ljet_ID_medium.clear();
        m_ljet_ID_tight.clear();
        m_ljet_ID_tightlepveto.clear();
        m_ljet_subjetIndex0.clear();
        m_ljet_subjetIndex1.clear();
        m_ljet_subjet_pt.clear();
        m_ljet_subjet_eta.clear();
        m_ljet_subjet_phi.clear();
        m_ljet_subjet_e.clear();
        m_ljet_subjet_CSV.clear();
        m_ljet_subjet_charge.clear();
        m_ljet_jec.clear();
        m_ljet_jer_up.clear();
        m_ljet_jer_down.clear();

        for (const auto& ljet : *m_ljets.product()){
            m_ljet_pt.push_back( ljet.p4.Pt());
            m_ljet_eta.push_back(ljet.p4.Eta());
            m_ljet_phi.push_back(ljet.p4.Phi());
            m_ljet_e.push_back(  ljet.p4.E());
            m_ljet_CSV.push_back(ljet.CSVv2);
            m_ljet_tau1.push_back(ljet.tau1);
            m_ljet_tau2.push_back(ljet.tau2);
            m_ljet_tau3.push_back(ljet.tau3);
            m_ljet_tau21.push_back(ljet.tau21);
            m_ljet_tau32.push_back(ljet.tau32);
            m_ljet_charge.push_back(ljet.charge);
            m_ljet_SDmass.push_back(ljet.softDropMass);
            m_ljet_ID_loose.push_back(ljet.loose);
            m_ljet_ID_medium.push_back(ljet.medium);
            m_ljet_ID_tight.push_back(ljet.tight);
            m_ljet_ID_tightlepveto.push_back(ljet.tightlepveto);
            m_ljet_subjetIndex0.push_back(ljet.vSubjetIndex0);
            m_ljet_subjetIndex1.push_back(ljet.vSubjetIndex1);
            m_ljet_jec.push_back(1);
            m_ljet_jer_up.push_back(1);
            m_ljet_jer_down.push_back(1);

            for (const auto& jet : ljet.subjets){
                m_ljet_subjet_pt.push_back( jet.p4.Pt());
                m_ljet_subjet_eta.push_back(jet.p4.Eta());
                m_ljet_subjet_phi.push_back(jet.p4.Phi());
                m_ljet_subjet_e.push_back(  jet.p4.E());
                m_ljet_subjet_CSV.push_back(jet.CSVv2);
                m_ljet_subjet_charge.push_back(jet.charge);
            }
        }
    }
    if (m_useLeptons){
        m_el_pt.clear();
        m_el_eta.clear();
        m_el_phi.clear();
        m_el_e.clear();
        m_el_iso.clear();
        m_el_charge.clear();
        m_el_ID_loose.clear();
        m_el_ID_medium.clear();
        m_el_ID_tight.clear();

        for (const auto& el : *m_electrons.product()){
            m_el_pt.push_back(  el.p4.Pt() );
            m_el_eta.push_back( el.p4.Eta() );
            m_el_phi.push_back( el.p4.Phi() );
            m_el_e.push_back(   el.p4.E() );
            m_el_iso.push_back( el.RelIsoEA );
            m_el_charge.push_back(    el.charge );
            m_el_ID_loose.push_back(  el.vidLoose );
            m_el_ID_medium.push_back( el.vidMedium );
            m_el_ID_tight.push_back(  el.vidTight );
        }

        m_mu_pt.clear();
        m_mu_eta.clear();
        m_mu_phi.clear();
        m_mu_e.clear();
        m_mu_iso.clear();
        m_mu_charge.clear();
        m_mu_ID_loose.clear();
        m_mu_ID_medium.clear();
        m_mu_ID_tight.clear();

        for (const auto& mu : *m_muons.product()){
            m_mu_pt.push_back(  mu.p4.Pt() );
            m_mu_eta.push_back( mu.p4.Eta() );
            m_mu_phi.push_back( mu.p4.Phi() );
            m_mu_e.push_back(   mu.p4.E() );
            m_mu_iso.push_back( mu.iso04 );
            m_mu_charge.push_back(    mu.charge );
            m_mu_ID_loose.push_back(  mu.loose );
            m_mu_ID_medium.push_back( (mu.loose && !mu.tight) ); // unofficial (30Jan2018)
            m_mu_ID_tight.push_back(  mu.tight );
        }
    }
    if (m_useNeutrinos){
        m_nu_pt.clear();
        m_nu_eta.clear();
        m_nu_phi.clear();

        for (const auto& nu : *m_neutrinos.product()){
            m_nu_pt.push_back(nu.p4.Pt());
            m_nu_eta.push_back(nu.p4.Eta());
            m_nu_phi.push_back(nu.p4.Phi());
        }
    }

    m_met_met = (*m_met.product()).p4.Pt();
    m_met_phi = (*m_met.product()).p4.Phi();

    m_runNumber   = event.id().run();   //*h_runno.product();
    m_eventNumber = event.id().event(); //*h_evtno.product();
    m_lumiblock   = event.id().luminosityBlock(); //*h_lumisec.product();
    m_npv = *h_npv.product();

    m_rho.clear();
    for (const auto& rho : *h_rho.product()){
        m_rho.push_back(rho);
    }
//    m_nGoodVtx = ;
//    m_LHA_PDF_ID = ;
//    m_nIsoTrk = ;

    m_met_met_sf = 1;
    m_met_phi_sf = 1;

    if (m_isMC){
/*
        m_true_pileup;
        m_weight_mc;
        m_weight_btag;
        m_weight_pileup;
        m_weight_jet_jer;
        m_weight_ljet_jer;
        m_xsection;
        m_kfactor;
        m_sumOfWeights;

        m_mc_pt;
        m_mc_eta;
        m_mc_phi;
        m_mc_e;
        m_mc_pdgId;
        m_mc_charge;
        m_mc_mom_idx;

        if (m_useJets){
            m_truth_jet_pt.clear();
            m_truth_jet_eta.clear();
            m_truth_jet_phi.clear();
            m_truth_jet_e.clear();
            m_truth_jet_charge.clear();
        }

        if (m_useLargeRJets){
            m_truth_ljet_pt.clear();
            m_truth_ljet_eta.clear();
            m_truth_ljet_phi.clear();
            m_truth_ljet_e.clear();
            m_truth_ljet_charge.clear();
        }
*/
    }

    // Fill output tree
    m_ttree->Fill();

    return;
}


void EventSaverFlatNtuple::endJob(){
    /* End of job 
       - Fill the metadata tree (only 1 "event")
    */
    // use getSampleWeights();
    m_sampleName = *h_sampleName.product();
    m_xsection   = *h_xsection.product();
    m_kfactor    = *h_kfactor.product();
    m_sumOfWeights = *h_sumOfWeights.product();

    m_metadata_ttree->Fill();

    return;
}


void EventSaverFlatNtuple::initialize_branches(){
    /* Setup the output trees */
    // Metadata
    m_metadata_ttree->Branch("sampleName",   &m_sampleName);                      // string
    m_metadata_ttree->Branch("xsection",     &m_xsection,     "xsection/F");      // float
    m_metadata_ttree->Branch("kfactor",      &m_kfactor,      "kfactor/F");       // float
    m_metadata_ttree->Branch("sumOfWeights", &m_sumOfWeights, "sumOfWeights/F");  // float

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
//    m_ttree->Branch("jet_true_flavor", &m_jet_true_flavor);   // vector of ints

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
    m_ttree->Branch("npv",             &m_npv,         "npv/I");            // uint
    m_ttree->Branch("nGoodVtx",        &m_nGoodVtx,    "nGoodVtx/I");       // int
    m_ttree->Branch("LHA_PDF_ID",      &m_LHA_PDF_ID,  "LHA_PDF_ID/I");     // int
    m_ttree->Branch("nIsoTrk",         &m_nIsoTrk,     "nIsoTrk/I");        // int
    m_ttree->Branch("true_pileup",     &m_true_pileup, "true_pileup/I");    // int

    // -- Weights
    m_ttree->Branch("weight_mc",       &m_weight_mc,      "weight_mc/F");          // float
    m_ttree->Branch("weight_btag",     &m_weight_btag,    "weight_btag/F");        // float
    m_ttree->Branch("weight_pileup",   &m_weight_pileup,  "weight_pileup/F");      // float
    m_ttree->Branch("weight_jet_jer",  &m_weight_jet_jer, "weight_jet_jer/F");     // float
    m_ttree->Branch("weight_ljet_jer", &m_weight_ljet_jer,"weight_ljet_jer/F");    // float
//    m_ttree->Branch("xsection",        &m_xsection,       "xsection/F");           // float
//    m_ttree->Branch("kfactor",         &m_kfactor,        "kfactor/F");            // float
//    m_ttree->Branch("sumOfWeights",    &m_sumOfWeights,   "sumOfWeights/F");       // float

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
