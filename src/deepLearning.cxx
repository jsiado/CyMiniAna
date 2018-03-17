/*
Created:        --
Last Updated:   19 February 2018

Dan Marley
daniel.edison.marley@cernSPAMNOT.ch
Texas A&M University

-----

Tool for performing deep learning tasks
*/
#include "Analysis/CyMiniAna/interface/deepLearning.h"


DeepLearning::DeepLearning( configuration& cmaConfig ) :
  m_config(&cmaConfig),
  m_lwnn(nullptr){
    // Setup lwtnn
    std::ifstream input_cfg = cma::open_file( m_config->dnnFile() );
    lwt::JSONConfig cfg     = lwt::parse_json( input_cfg );
    m_lwnn   = new lwt::LightweightNeuralNetwork(cfg.inputs, cfg.layers, cfg.outputs);
    m_dnnKey = m_config->dnnKey();
  }

DeepLearning::~DeepLearning() {
    delete m_lwnn;
}


void DeepLearning::training(){
    /* Prepare inputs for training */
    loadFeatures();

    return;
}

void DeepLearning::inference(){
    /* Obtain results from LWTNN */
    loadFeatures();
    m_discriminant = m_lwnn->compute(m_dnnInputs);

    return;
}


void DeepLearning::loadFeatures(){
    /* Calculate DNN features */
    m_dnnInputs.clear();

    // feature calculations
    m_dnnInputs["target"] = 1;
    cma::DEBUG("EVENT : Set DNN input values ");

    return;
}

std::map<std::string,double> DeepLearning::features(){
    /* return features */
    return m_dnnInputs;
}

std::map<std::string,double> DeepLearning::predictions(){
    /* Return the full map to the user */
    return m_discriminant;
}

double DeepLearning::prediction(){
    /* Return the score for the default key */
    return m_discriminant.at(m_dnnKey);
}

double DeepLearning::prediction(const std::string& key){
    /* Just return the prediction (after execute!) */
    return m_discriminant.at(key);
}

// THE END //
