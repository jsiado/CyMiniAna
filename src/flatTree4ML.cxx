/*
Created:        --
Last Updated:    7 May 2018

Dan Marley
daniel.edison.marley@cernSPAMNOT.ch
Texas A&M University

-----

Create and fill TTree for ML
*/
#include "Analysis/CyMiniAna/interface/flatTree4ML.h"


flatTree4ML::flatTree4ML(configuration &cmaConfig) : 
  m_config(&cmaConfig){}

flatTree4ML::~flatTree4ML() {}


void flatTree4ML::initialize(TFile& outputFile) {
    /*
       Setup the new tree 
       Contains features for the NN
       --  No vector<T> stored in tree: completely flat!
    */
    outputFile.cd();                                     // move to output file
    m_ttree        = new TTree("features", "features");  // Tree contains features for the NN
    m_metadataTree = new TTree("metadata","metadata");   // Tree contains metadata

    /**** Setup new branches here ****/
    // Weights
    m_ttree->Branch( "xsection", &m_xsection, "xsection/F" );
    m_ttree->Branch( "kfactor",  &m_kfactor,  "kfactor/F" );
    m_ttree->Branch( "weight",   &m_weight,   "weight/F" );
    m_ttree->Branch( "sumOfWeights",   &m_sumOfWeights,   "sumOfWeights/F" );
    m_ttree->Branch( "nominal_weight", &m_nominal_weight, "nominal_weight/F" );

    // Features
    m_ttree->Branch( "target", &m_target, "target/F" );  // target value (neutrino pz)

    m_ttree->Branch( "met_met", &m_met_met, "met_met/F");
    m_ttree->Branch( "met_phi", &m_met_phi, "met_phi/F");
    m_ttree->Branch( "mtw",     &m_mtw,     "mtw/F");
    m_ttree->Branch( "lepton_pt",  &m_lepton_pt, "lepton_pt/F");
    m_ttree->Branch( "lepton_eta", &m_lepton_eta, "lepton_eta/F");
    m_ttree->Branch( "deltaPhi_lep_met", &m_deltaPhi_lep_met, "deltaPhi_lep_met/F");

    m_ttree->Branch( "n_jets", &m_n_jets, "n_jets/I");
    m_ttree->Branch( "deltaPhi_j0_met_phi", &m_deltaPhi_j0_met_phi, "deltaPhi_j0_met_phi/F");
    m_ttree->Branch( "deltaPhi_j1_met_phi", &m_deltaPhi_j1_met_phi, "deltaPhi_j1_met_phi/F");
    m_ttree->Branch( "deltaPhi_j2_met_phi", &m_deltaPhi_j2_met_phi, "deltaPhi_j2_met_phi/F");
    m_ttree->Branch( "deltaPhi_j3_met_phi", &m_deltaPhi_j3_met_phi, "deltaPhi_j3_met_phi/F");
    m_ttree->Branch( "jet0_bdisc", &m_jet0_bdisc, "jet0_bdisc/F");
    m_ttree->Branch( "jet1_bdisc", &m_jet1_bdisc, "jet1_bdisc/F");
    m_ttree->Branch( "jet2_bdisc", &m_jet2_bdisc, "jet2_bdisc/F");
    m_ttree->Branch( "jet3_bdisc", &m_jet3_bdisc, "jet3_bdisc/F");
    m_ttree->Branch( "jet0_ptrel", &m_jet0_ptrel, "jet0_ptrel/F");
    m_ttree->Branch( "jet1_ptrel", &m_jet1_ptrel, "jet1_ptrel/F");
    m_ttree->Branch( "jet2_ptrel", &m_jet2_ptrel, "jet2_ptrel/F");
    m_ttree->Branch( "jet3_ptrel", &m_jet3_ptrel, "jet3_ptrel/F");

    m_ttree->Branch( "nu_pz_standard", &m_nu_pz_standard, "nu_pz_standard/F");
    m_ttree->Branch( "nu_pz_sampling", &m_nu_pz_sampling, "nu_pz_sampling/F");

    /**** Metadata ****/
    // which sample has which target value
    // many ROOT files will be merged together to do the training
    m_metadataTree->Branch( "name",    &m_name );
    m_metadataTree->Branch( "target",  &m_target_value,  "target/I" );
    m_metadataTree->Branch( "nEvents", &m_nEvents,       "nEvents/I" );

    return;
} // end initialize



void flatTree4ML::saveEvent(const std::map<std::string,double> features) {
    /* Save the ML features to the ttree! */
    cma::DEBUG("FLATTREE4ML : Save event ");

    m_weight   = features.at("weight");
    m_kfactor  = features.at("kfactor");
    m_xsection = features.at("xsection");
    m_sumOfWeights = features.at("sumOfWeights");
    m_nominal_weight = features.at("nominal_weight");

    m_target = features.at("target");

    m_met_met = features.at("met_met");
    m_met_phi = features.at("met_phi");
    m_mtw     = features.at("mtw");
    m_lepton_pt  = features.at("lepton_pt");
    m_lepton_eta = features.at("lepton_eta");
    m_deltaPhi_lep_met = features.at("deltaPhi_lep_met");

    m_n_jets = features.at("n_jets");
    m_deltaPhi_j0_met_phi = features.at("deltaPhi_j0_met_phi");
    m_deltaPhi_j1_met_phi = features.at("deltaPhi_j1_met_phi");
    m_deltaPhi_j2_met_phi = features.at("deltaPhi_j2_met_phi");
    m_deltaPhi_j3_met_phi = features.at("deltaPhi_j3_met_phi");

    m_jet0_bdisc = features.at("jet0_bdisc");
    m_jet1_bdisc = features.at("jet1_bdisc");
    m_jet2_bdisc = features.at("jet2_bdisc");
    m_jet3_bdisc = features.at("jet3_bdisc");

    m_jet0_ptrel = features.at("jet0_ptrel");
    m_jet1_ptrel = features.at("jet1_ptrel");
    m_jet2_ptrel = features.at("jet2_ptrel");
    m_jet3_ptrel = features.at("jet3_ptrel");

    m_nu_pz_standard = features.at("pz_standard");
    m_nu_pz_sampling = features.at("pz_sampling");

    /**** Fill the tree ****/
    cma::DEBUG("FLATTREE4ML : Fill the tree");
    m_ttree->Fill();

    return;
}


void flatTree4ML::finalize(){
    /* Finalize the class -- fill in the metadata (only need to do this once!) */
    m_name    = m_config->primaryDataset();
    m_nEvents = m_config->NTotalEvents();
    m_target_value = 0;

    cma::DEBUG("FLATTREE4ML : Fill the metadata tree");
    m_metadataTree->Fill();
}

// THE END
