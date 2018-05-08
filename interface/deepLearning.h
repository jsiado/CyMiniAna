#ifndef DEEPLEARNING_H
#define DEEPLEARNING_H

#include <string>
#include <map>
#include <vector>

#include "lwtnn/lwtnn/interface/LightweightNeuralNetwork.hh"
#include "lwtnn/lwtnn/interface/parse_json.hh"

#include "Analysis/CyMiniAna/interface/tools.h"
#include "Analysis/CyMiniAna/interface/configuration.h"
#include "Analysis/CyMiniAna/interface/physicsObjects.h"


class DeepLearning {
  public:
    DeepLearning( configuration& cmaConfig );

    ~DeepLearning();
    void clear();

    void training();
    void inference();
    void loadFeatures();

    std::map<std::string,double> predictions() const {return m_predictions;}
    double prediction() const {return m_DNN;}
    double prediction(const std::string& key) const;

    std::map<std::string,double> features() const {return m_features;}

    void setTrueNeutrino(Parton& nu);
    void setLepton(Lepton& lep);
    void setMET(MET& etmiss);
    void setJets(std::vector<Jet> jets);

  protected:

    configuration *m_config;

    Lepton m_lepton;
    MET m_met;
    Parton m_true_neutrino;
    std::vector<Jet> m_jets;
    std::vector<Ljet> m_ljets;

    lwt::LightweightNeuralNetwork* m_lwnn;       // LWTNN tool

    std::map<std::string, double> m_features;    // values for inputs to the DNN
    std::map<std::string,double> m_predictions;  // map of DNN predictions
    std::string m_dnnKey;                        // default key for accessing map of values
    float m_DNN;                                 // DNN prediction for one key

    std::map<std::string,double> m_discriminant; // map of DNN predictions
};

#endif

