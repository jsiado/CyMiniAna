/*
Created:        15 March 2018
Last Updated:   15 March 2018

Dan Marley
daniel.edison.marley@cernSPAMNOT.ch
Texas A&M University
-----

Make histograms of deep learning features 
*/
#include "Analysis/CyMiniAna/interface/histogrammer4ML.h"


histogrammer4ML::histogrammer4ML( configuration& cmaConfig, std::string name ) :
  histogrammer::histogrammer(cmaConfig,name),
  m_config(&cmaConfig),
  m_name(name){}

histogrammer4ML::~histogrammer4ML() {}


/**** INITIALIZE HISTOGRAMS ****/


void histogrammer4ML::initialize( TFile& outputFile ){
    /* Setup some values and book histograms */
    outputFile.cd();

    bookHists();

    return;
}

void histogrammer4ML::bookHists(){
    /* 
      Book histograms -- modify/inherit this function for analysis-specific hists 

      @param name   This is the string used to identify histograms for different systematics/event weights
    */
    cma::DEBUG("HISTOGRAMMER : Init. histograms: "+m_name);

    histogrammer::init_hist( "met_met_"+m_name,    500, 0.0, 1000);
    histogrammer::init_hist( "met_phi_"+m_name,     64, -3.2, 3.2);
    histogrammer::init_hist( "mtw_"+m_name,        100, 0.0,  500);
    histogrammer::init_hist( "lepton_pt_"+m_name,  500, 0.0, 2000);
    histogrammer::init_hist( "lepton_eta_"+m_name,  50, -2.5, 2.5);
    histogrammer::init_hist( "deltaPhi_lep_met_"+m_name, 16, -4, 4);

    histogrammer::init_hist( "n_jets_"+m_name, 31, -0.5,  30.5 );
    for (unsigned int b=0; b<4; b++){
        std::string sb = std::to_string(b);
        histogrammer::init_hist( "deltaPhi_j"+sb+"_met_phi_"+m_name, 16, -4, 4);
        histogrammer::init_hist( "jet"+sb+"_bdisc_"+m_name, 50, 0, 1);
        histogrammer::init_hist( "jet"+sb+"_ptrel_"+m_name, 50, 0, 1);
    }

    return;
}


/**** FILL HISTOGRAMS ****/
void histogrammer4ML::fill( const std::map<std::string,double> features, double weight ){
    /* Fill histograms -- 
       Fill information from single top object (inputs to deep learning)
    */
    cma::DEBUG("HISTOGRAMMER : Fill histograms: "+m_name);

    histogrammer::fill( "met_met_"+m_name,    features.at("met_met"), weight);
    histogrammer::fill( "met_phi_"+m_name,    features.at("met_phi"), weight);
    histogrammer::fill( "mtw_"+m_name,        features.at("mtw"), weight);
    histogrammer::fill( "lepton_pt_"+m_name,  features.at("lepton_pt"),  weight);
    histogrammer::fill( "lepton_eta_"+m_name, features.at("lepton_eta"), weight);
    histogrammer::fill( "deltaPhi_lep_met_"+m_name, features.at("deltaPhi_lep_met"), weight);

    histogrammer::fill( "n_jets_"+m_name, features.at("n_jets"), weight);
    for (unsigned int b=0; b<4; b++){
        std::string sb = std::to_string(b);
        histogrammer::fill( "deltaPhi_j"+sb+"_met_phi_"+m_name, features.at("deltaPhi_j"+sb+"_met_phi"), weight);
        histogrammer::fill( "jet"+sb+"_bdisc_"+m_name, features.at("jet"+sb+"_bdisc"), weight);
        histogrammer::fill( "jet"+sb+"_ptrel_"+m_name, features.at("jet"+sb+"_ptrel"), weight);
    }

    cma::DEBUG("HISTOGRAMMER : End histograms");

    return;
}

// THE END

