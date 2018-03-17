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
  histogrammerFlatNtuple::histogrammerFlatNtuple(cmaConfig,name),
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

    histogrammerFlatNtuple::init_hist("feature_"+m_name,  5000,0,5000,50,0,5);

    return;
}


/**** FILL HISTOGRAMS ****/
void histogrammer4ML::fill( const std::map<std::string,double> features, double weight ){
    /* Fill histograms -- 
       Fill information from single top object (inputs to deep learning)
    */
    cma::DEBUG("HISTOGRAMMER : Fill histograms: "+m_name);

    histogrammerFlatNtuple::fill("feature_"+m_name,  features.at("feature"), weight );

    cma::DEBUG("HISTOGRAMMER : End histograms");

    return;
}

// THE END

