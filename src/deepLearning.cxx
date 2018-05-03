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
    m_features.clear();

    // Setup lwtnn
    m_dnnKey = m_config->dnnKey();
    if (m_config->DNNinference()){
      std::ifstream input_cfg = cma::open_file( m_config->dnnFile() );
      lwt::JSONConfig cfg     = lwt::parse_json( input_cfg );
      m_lwnn   = new lwt::LightweightNeuralNetwork(cfg.inputs, cfg.layers, cfg.outputs);
    }
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
    m_discriminant = m_lwnn->compute(m_features);

    return;
}


void DeepLearning::loadFeatures(){
    /* Calculate DNN features */
    m_features.clear();

    // feature calculations
    m_features["target"] = 1;
    cma::DEBUG("EVENT : Set DNN input values ");

    return;
}

double DeepLearning::prediction(const std::string& key) const{
    /* Just return the prediction (after execute!) */
    return m_predictions.at(key);
}

// THE END //
