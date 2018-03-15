#ifndef EVENT_H
#define EVENT_H

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TSystem.h"
#include "TEfficiency.h"
#include "TMath.h"
#include "TLorentzVector.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"
#include "TParameter.h"
#include "TEnv.h"
#include "TF1.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <memory>
#include <set>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "Analysis/CyMiniAna/interface/physicsObjects.h"
#include "Analysis/CyMiniAna/interface/configuration.h"


// Event Class
class Event {
  public:
    // Constructor
    Event( TTreeReader &myReader, configuration &cmaConfig);
    Event( const Event &obj);

    // Destructor
    virtual ~Event();

    // create hash tables in truth/reco TTree to match truth <-> reco events
    // uses configuration option matchTruthToReco to match truth to reco (reco loop)
    // OR match reco to truth (truth loop, for acceptance studies)
    void matchTruthWithReco();
    // check during looping over truth events, if reco event match is found
    bool isValidRecoEntry();

    // Execute the event (load information and setup objects)
    virtual void execute(Long64_t entry);
    virtual void updateEntry(Long64_t entry);

    // Setup physics information
    void initialize_leptons();
    void initialize_neutrinos();
    void initialize_jets();
    void initialize_ljets();
    void initialize_eventWeights();
    void initialize_weights();
    void initialize_kinematics();
    void initialize_truth();

    virtual double getSystEventWeight(const std::string &syst, const int weightIndex=-1);

    // Clear stuff;
    virtual void finalize();
    virtual void clear();

    // Get physics information
    std::vector<Lepton> leptons();
    std::vector<Neutrino> neutrinos();
    std::vector<Ljet> ljets();
    std::vector<Jet>  jets();

    // Get truth physics information 
    std::vector<Lepton> truth_leptons();
    std::vector<Neutrino> truth_neutrinos();
    std::vector<Ljet> truth_ljets();
    std::vector<Jet>  truth_jets();

    virtual float met( const std::string& met_name );
    virtual float HT();
    virtual float ST();
    virtual void getBtaggedJets( Jet& jet );
    virtual std::vector<int> btag_jets(const std::string &wkpt);
    virtual std::vector<int> btag_jets(); // using configured b-tag WP

//    virtual unsigned long long eventNumber();
//    virtual unsigned int runNumber();
    long long entry() { return m_entry; }
    virtual float eventNumber();
    virtual float runNumber();
    virtual float mu();
    virtual int lumiblock();
    virtual std::string treeName();
    virtual float xsection();
    virtual float kfactor();
    virtual float sumOfWeights();
    void truth();

    // Neural network information
    void getDNNInputs();      // return the DNN inputs to the user
    void getDNN();            // get the DNN output
    double DNN();

    // kinematic reconstruction
    void buildTtbar();

    // Get weights
    virtual float nominal_weight();
    float weight_mc();
    float truth_weight_mc();
    float weight_jvt();
    float weight_pileup();
    float weight_lept_eff();
    float weight_btag();
    float weight_btag(const std::string &wkpt);

    // Get weight systematics
    virtual std::map<std::string,float > weightSystematicsFloats();
    virtual std::map<std::string,std::vector<float> > weightSystematicsVectorFloats();
    virtual std::vector<std::string> listOfWeightSystematics();

  protected:

    // general information
    configuration *m_config;
    TTreeReader &m_ttree;
    TTreeReader m_truth_tree;
    std::string m_treeName;
    std::string m_fileName;
    bool m_grid;
    bool m_isMC;
    long long m_entry;
    long long m_truth_entry;

    // 0/1/2-lepton analyses
    bool m_isZeroLeptonAnalysis;
    bool m_isOneLeptonAnalysis;
    bool m_isTwoLeptonAnalysis;

    // neural network & kinematic reconstruction
    bool m_kinematicReco;
    bool m_buildNeutrinos;
    bool m_getDNN;
    double m_DNN;                                // DNN score

    // event weight information
    double m_nominal_weight;
    double m_xsection;
    double m_kfactor;
    double m_sumOfWeights;
    double m_LUMI;
    std::map<int, float> m_mapXSection; // map DSID to XSection
    std::map<int, float> m_mapKFactor;  // map DSID to KFactor
    std::map<int, float> m_mapAMI;      // map DSID to sum of weights

    // physics object information
    std::vector<Lepton> m_leptons;
    std::vector<Neutrino> m_neutrinos;
    std::vector<Ljet> m_ljets;
    std::vector<Jet>  m_jets;

    // truth physics object information
    std::vector<Lepton> m_truth_leptons;
    std::vector<Neutrino> m_truth_neutrinos;
    std::vector<Ljet> m_truth_ljets;
    std::vector<Jet>  m_truth_jets;

    // b-tagged calo jets with various WP
    std::map<std::string, std::vector<int> > m_btag_jets;
    std::vector<int> m_btag_jets_default;
    float m_cMVAv2L;
    float m_cMVAv2M;
    float m_cMVAv2T;
    float m_CSVv2L;
    float m_CSVv2M;
    float m_CSVv2T;

    // kinematics
    float m_HT;
    float m_ST;
    float m_metmet;
    float m_metphi;

    // nominal b-tagging weight maps
    std::map<std::string, float> m_weight_btag;
    float m_weight_btag_default;
    // Maps to keep track of weight systematics
    std::map<std::string,TTreeReaderValue<float> * > m_weightSystematicsFloats;
    std::map<std::string,TTreeReaderArray<float> * > m_weightSystematicsVectorFloats;
    std::vector<std::string> m_listOfWeightSystematics;




    // TTree variables [all possible ones]
    // ************
    TTreeReaderValue<float> * m_weight_mc;
    TTreeReaderValue<float> * m_weight_pileup;
    TTreeReaderValue<float> * m_weight_btag;
    TTreeReaderValue<float> * m_weight_pileup_UP;
    TTreeReaderValue<float> * m_weight_pileup_DOWN;

    // Event info 
    TTreeReaderValue<int> * m_eventNumber;
    TTreeReaderValue<int> * m_runNumber;
    TTreeReaderValue<int> * m_lumiblock;
    TTreeReaderValue<float> * m_rho;
    TTreeReaderValue<int> * m_npv;
    TTreeReaderArray<int> * m_NGoodVtx;
    TTreeReaderArray<int> * m_LHAPDF_ID;
    TTreeReaderArray<int> * m_NIsoTrk;
    TTreeReaderArray<int> * m_true_pileup;
    TTreeReaderValue<float> * m_treeXSection;
    TTreeReaderValue<float> * m_treeKFactor;
    TTreeReaderValue<float> * m_treeSumOfWeights;

    TTreeReaderValue<float> * m_dnn_score;

    // MET
    TTreeReaderArray<float> * m_met_met;
    TTreeReaderArray<float> * m_met_phi;
    TTreeReaderArray<float> * m_met_met_sf;
    TTreeReaderArray<float> * m_met_phi_sf;

    // HT/ST
    TTreeReaderArray<float> * m_treeHT;
    TTreeReaderArray<float> * m_treeST;

    // Leptons
    TTreeReaderArray<float> * m_el_pt;
    TTreeReaderArray<float> * m_el_eta;
    TTreeReaderArray<float> * m_el_phi;
    TTreeReaderArray<float> * m_el_e;
    TTreeReaderArray<float> * m_el_charge;
    TTreeReaderArray<float> * m_el_iso;
    TTreeReaderArray<float> * m_el_veto;
    TTreeReaderArray<float> * m_el_loose;
    TTreeReaderArray<float> * m_el_medium;
    TTreeReaderArray<float> * m_el_tight;

    TTreeReaderArray<float> * m_mu_pt;
    TTreeReaderArray<float> * m_mu_eta;
    TTreeReaderArray<float> * m_mu_phi;
    TTreeReaderArray<float> * m_mu_e;
    TTreeReaderArray<float> * m_mu_charge;
    TTreeReaderArray<float> * m_mu_iso;
    TTreeReaderArray<int> * m_mu_loose;
    TTreeReaderArray<int> * m_mu_medium;
    TTreeReaderArray<int> * m_mu_medium2016;
    TTreeReaderArray<int> * m_mu_tight;
    TTreeReaderArray<int> * m_mu_hightPt;

    // large-R jet info
    TTreeReaderValue<float> * m_weight_ljet_jer;

    TTreeReaderArray<std::vector<float>> * m_ljet_pt;
    TTreeReaderArray<std::vector<float>> * m_ljet_eta;
    TTreeReaderArray<std::vector<float>> * m_ljet_phi;
    TTreeReaderArray<std::vector<float>> * m_ljet_e;
    TTreeReaderArray<std::vector<float>> * m_ljet_tau1;
    TTreeReaderArray<std::vector<float>> * m_ljet_tau2;
    TTreeReaderArray<std::vector<float>> * m_ljet_tau3;
    TTreeReaderArray<std::vector<float>> * m_ljet_tau21;
    TTreeReaderArray<std::vector<float>> * m_ljet_tau32;
    TTreeReaderArray<std::vector<float>> * m_ljet_charge;
    TTreeReaderArray<std::vector<float>> * m_ljet_SDmass;
    TTreeReaderArray<std::vector<float>> * m_ljet_CSVv2;
    TTreeReaderArray<std::vector<float>> * m_ljet_JERSF;
    TTreeReaderArray<std::vector<float>> * m_ljet_JERSFUp;
    TTreeReaderArray<std::vector<float>> * m_ljet_JERSFDown;
    TTreeReaderArray<std::vector<int>> * m_ljet_ID_loose;
    TTreeReaderArray<std::vector<int>> * m_ljet_ID_medium;
    TTreeReaderArray<std::vector<int>> * m_ljet_ID_tight;
    TTreeReaderArray<std::vector<int>> * m_ljet_ID_tightlepveto;

    TTreeReaderArray<std::vector<float>> * m_ljet_subjetIndex0;
    TTreeReaderArray<std::vector<float>> * m_ljet_subjetIndex1;
    TTreeReaderArray<std::vector<float>> * m_ljet_subjet_pt;
    TTreeReaderArray<std::vector<float>> * m_ljet_subjet_eta;
    TTreeReaderArray<std::vector<float>> * m_ljet_subjet_phi;
    TTreeReaderArray<std::vector<float>> * m_ljet_subjet_e;
    TTreeReaderArray<std::vector<float>> * m_ljet_subjet_charge;
    TTreeReaderArray<std::vector<float>> * m_ljet_subjet_CSVv2;
    TTreeReaderArray<std::vector<float>> * m_ljet_jec;
    TTreeReaderArray<std::vector<float>> * m_ljet_jer_up;
    TTreeReaderArray<std::vector<float>> * m_ljet_jer_down;

    // truth large-R jet info
    TTreeReaderArray<std::vector<float>> * m_truth_ljet_pt;
    TTreeReaderArray<std::vector<float>> * m_truth_ljet_eta;
    TTreeReaderArray<std::vector<float>> * m_truth_ljet_phi;
    TTreeReaderArray<std::vector<float>> * m_truth_ljet_e;
    TTreeReaderArray<std::vector<float>> * m_truth_ljet_charge;
    TTreeReaderArray<std::vector<float>> * m_truth_ljet_subjet_pt;
    TTreeReaderArray<std::vector<float>> * m_truth_ljet_subjet_eta;
    TTreeReaderArray<std::vector<float>> * m_truth_ljet_subjet_phi;
    TTreeReaderArray<std::vector<float>> * m_truth_ljet_subjet_e;
    TTreeReaderArray<std::vector<float>> * m_truth_ljet_subjet_charge;

    // Jet info
    TTreeReaderValue<float> * m_weight_jet_jer;

    TTreeReaderArray<std::vector<float>> * m_jet_pt;
    TTreeReaderArray<std::vector<float>> * m_jet_eta;
    TTreeReaderArray<std::vector<float>> * m_jet_phi;
    TTreeReaderArray<std::vector<float>> * m_jet_e;
    TTreeReaderArray<std::vector<float>> * m_jet_charge;
    TTreeReaderArray<std::vector<float>> * m_jet_CSVv2;
    TTreeReaderArray<std::vector<int>> * m_jet_ID_loose;
    TTreeReaderArray<std::vector<int>> * m_jet_ID_medium;
    TTreeReaderArray<std::vector<int>> * m_jet_ID_tight;
    TTreeReaderArray<std::vector<int>> * m_jet_ID_tightlepveto;
    TTreeReaderArray<std::vector<float>> * m_jet_jec;
    TTreeReaderArray<std::vector<float>> * m_jet_jer_up;
    TTreeReaderArray<std::vector<float>> * m_jet_jer_down;

    // truth jet info
    TTreeReaderArray<std::vector<float>> * m_truth_jet_pt;
    TTreeReaderArray<std::vector<float>> * m_truth_jet_eta;
    TTreeReaderArray<std::vector<float>> * m_truth_jet_phi;
    TTreeReaderArray<std::vector<float>> * m_truth_jet_e;
    TTreeReaderArray<std::vector<float>> * m_truth_jet_charge;

    // Reconstructed neutrino
    TTreeReaderValue<std::vector<float>> * m_nu_pt;
    TTreeReaderValue<std::vector<float>> * m_nu_eta;
    TTreeReaderValue<std::vector<float>> * m_nu_phi;
    TTreeReaderValue<std::vector<float>> * m_nu_e;

    // Reconstructed VLQ, Wprime
    TTreeReaderValue<float> * m_vlq_pt;
    TTreeReaderValue<float> * m_vlq_eta;
    TTreeReaderValue<float> * m_vlq_phi;
    TTreeReaderValue<float> * m_vlq_e;
    TTreeReaderValue<int> * m_jet_vlq_index;
    TTreeReaderValue<float> * m_wprime_pt;
    TTreeReaderValue<float> * m_wprime_eta;
    TTreeReaderValue<float> * m_wprime_phi;
    TTreeReaderValue<float> * m_wprime_e;
    TTreeReaderValue<int> * m_jet_wprime_index;


    // General truth information
    TTreeReaderArray<std::vector<float>> * mc_pt;
    TTreeReaderArray<std::vector<float>> * mc_eta;
    TTreeReaderArray<std::vector<float>> * mc_phi;
    TTreeReaderArray<std::vector<float>> * mc_e;
    TTreeReaderArray<std::vector<float>> * mc_pdgId;
    TTreeReaderArray<std::vector<float>> * mc_charge;
    TTreeReaderArray<std::vector<int>> * mc_mom_idx;

    TTreeReaderValue<float> * m_mc_ht;                // evt_Gen_Ht

    // HLT 
    TTreeReaderArray<int> * m_HLT_Ele45_WPLoose_Gsf;          // HLT_Ele45_WPLoose_Gsf
    TTreeReaderArray<int> * m_HLT_Mu50;            // HLT_Mu50
    TTreeReaderArray<int> * m_HLT_TkMu50;          // HLT_TkMu50
};

#endif
