/*
Created:        --
Last Updated:   19 February 2018

Dan Marley
daniel.edison.marley@cernSPAMNOT.ch
Texas A&M University

-----

Tool for performing deep learning tasks
 - Regression analysis of neutrino pz
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


void DeepLearning::clear(){
    /* Clear data members */
    m_features.clear();
    m_lepton.clear();
    m_true_neutrino.clear();
    m_met.clear();
    m_jets.clear();

    return;
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


void DeepLearning::setTrueNeutrino(Parton& nu){
    /* Set the parton-level neutrino for the event */
    m_true_neutrino = nu;
    return;
}

void DeepLearning::setLepton(Lepton& lep){
    /* Set the lepton for the event */
    m_lepton = lep;
    return;
}

void DeepLearning::setMET(MET& etmiss){
    /* Set the MET for the event */
    m_met = etmiss;
    return;
}

void DeepLearning::setJets(std::vector<Jet> jets){
    /* Set the jets for the event */
    m_jets = jets;
    return;
}


void DeepLearning::loadFeatures(){
    /* Calculate DNN features */
    m_features.clear();

    // feature calculations
    // LEPTON & MET
    float met_met = m_met.p4.Pt();
    m_features["met_met"] = met_met;
    m_features["met_phi"] = m_met.p4.Phi();
    m_features["mtw"]     = m_met.mtw;
    m_features["lepton_pt"]  = m_lepton.p4.Pt();
    m_features["lepton_eta"] = m_lepton.p4.Eta();
    m_features["deltaPhi_lep_met"] = m_lepton.p4.DeltaPhi( m_met.p4 );

    // JETS & MET
    unsigned int n_jets  = m_jets.size();
    m_features["n_jets"] = n_jets;

    // DeltaPhi(j,nu) = pi (default value if j doesn't exist)
    m_features["deltaPhi_j0_met_phi"] = M_PI;
    m_features["deltaPhi_j1_met_phi"] = M_PI;
    m_features["deltaPhi_j2_met_phi"] = M_PI;
    m_features["deltaPhi_j3_met_phi"] = M_PI;
    // jets b-disc = -1 (default if it doesn't exist)
    m_features["jet0_bdisc"] = -1;
    m_features["jet1_bdisc"] = -1;
    m_features["jet2_bdisc"] = -1;
    m_features["jet3_bdisc"] = -1;
    // jets pt_rel = 0 (default if jet doesn't exist)
    m_features["jet0_ptrel"] = 0;
    m_features["jet1_ptrel"] = 0;
    m_features["jet2_ptrel"] = 0;
    m_features["jet3_ptrel"] = 0;

    if (n_jets>0){
        m_features["deltaPhi_j0_met_phi"] = m_jets.at(0).p4.DeltaPhi( m_met.p4 );
        m_features["jet0_bdisc"] = m_jets.at(0).bdisc;
        m_features["jet0_ptrel"] = m_jets.at(0).p4.Pt() / (m_jets.at(0).p4.Pt() + met_met);
        if (n_jets>1){
            m_features["deltaPhi_j1_met_phi"] = m_jets.at(1).p4.DeltaPhi(m_met.p4);
            m_features["jet1_bdisc"] = m_jets.at(1).bdisc;
            m_features["jet1_ptrel"] = m_jets.at(1).p4.Pt() / (m_jets.at(1).p4.Pt() + met_met);
            if (n_jets>2){
                m_features["deltaPhi_j2_met_phi"] = m_jets.at(2).p4.DeltaPhi(m_met.p4);
                m_features["jet2_bdisc"] = m_jets.at(2).bdisc;
                m_features["jet2_ptrel"] = m_jets.at(2).p4.Pt() / (m_jets.at(2).p4.Pt() + met_met);
                if (n_jets>3){
                    m_features["deltaPhi_j3_met_phi"] = m_jets.at(3).p4.DeltaPhi(m_met.p4);
                    m_features["jet3_bdisc"] = m_jets.at(3).bdisc;
                    m_features["jet3_ptrel"] = m_jets.at(3).p4.Pt() / (m_jets.at(3).p4.Pt() + met_met);
                } // end at least 4 jets
            } // end at least 3 jets
        } // end at least 2 jets
    } // end at least 1 jet

    m_features["target"] = m_true_neutrino.p4.Pz();
    cma::DEBUG("EVENT : Set DNN input values ");

    return;
}

double DeepLearning::prediction(const std::string& key) const{
    /* Just return the prediction (after execute!) */
    return m_predictions.at(key);
}

// THE END //
