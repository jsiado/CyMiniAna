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
#include "Analysis/CyMiniAna/interface/truthMatching.h"
#include "Analysis/CyMiniAna/interface/deepLearning.h"
#include "Analysis/CyMiniAna/interface/neutrinoReco.h"
#include "Analysis/CyMiniAna/interface/wprimeReco.h"


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
    bool isValidRecoEntry() {return (m_entry > (long long)-1);}

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
    std::vector<Lepton> leptons() {return m_leptons;}
    std::vector<Muon> muons() {return m_muons;}
    std::vector<Electron> electrons() {return m_electrons;}
    std::vector<Neutrino> neutrinos() {return m_neutrinos;}
    std::vector<Ljet> ljets() {return m_ljets;}
    std::vector<Jet>  jets() {return m_jets;}

    // Get truth physics information 
    std::vector<Lepton> truth_leptons() {return m_truth_leptons;}
    std::vector<Neutrino> truth_neutrinos() {return m_truth_neutrinos;}
    std::vector<Ljet> truth_ljets() {return m_truth_ljets;}
    std::vector<Jet>  truth_jets() {return m_truth_jets;}

    virtual float met( const std::string& met_name );
    virtual float HT() {return m_HT;}
    virtual float ST() {return m_ST;}
    virtual void getBtaggedJets( Jet& jet );
    virtual std::vector<int> btag_jets(const std::string &wkpt);
    virtual std::vector<int> btag_jets() {return m_btag_jets_default;}  // using configured b-tag WP

    long long entry() { return m_entry; }
    virtual int eventNumber();
    virtual int runNumber();
    virtual int lumiblock();
    virtual std::string treeName() {return m_treeName;}
    virtual float xsection() {return m_xsection;}
    virtual float kfactor() {return m_kfactor;}
    virtual float sumOfWeights() {return m_sumOfWeights;}

    // Functions for external tools/information
    void truth();
    void wprimeReconstruction();    // reconstructing Wprime (interface with tool)
    void deepLearning();            // DNN inference or training
    float DNN() {return m_DNN;}     // return DNN value (if only one)

    // Get weights
    virtual float nominal_weight() {return m_nominal_weight;}
    float weight_mc();
    float truth_weight_mc();
    float weight_pileup();
    float weight_lept_eff();
    float weight_btag() {return m_weight_btag_default;}
    float weight_btag(const std::string &wkpt);

    // Get weight systematics
    virtual std::map<std::string,float > weightSystematicsFloats();
    virtual std::map<std::string,std::vector<float> > weightSystematicsVectorFloats();

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

    bool m_useTruth;
    bool m_useJets;
    bool m_useLjets;
    bool m_useLeptons;
    bool m_useNeutrinos;
    bool m_neutrinoReco;
    bool m_useWprime;
    bool m_wprimeReco;
    bool m_DNNinference;
    bool m_DNNtraining;
    bool m_useDNN;
    bool m_getDNN;
    float m_DNN;                       // DNN score


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
    std::vector<Muon> m_muons;
    std::vector<Electron> m_electrons;
    std::vector<Neutrino> m_neutrinos;
    std::vector<Ljet> m_ljets;
    std::vector<Jet>  m_jets;

    // truth physics object information
    std::vector<Parton> m_truth_partons;
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
    std::map<std::string,TTreeReaderValue<std::vector<float>> * > m_weightSystematicsVectorFloats;
    std::vector<std::string> m_listOfWeightSystematics;

    // External tools
    NeutrinoReco* m_neutrinoTool;
    WprimeReco* m_wprimeTool;
    DeepLearning* m_deepLearningTool;
    truthMatching* m_truthMatchingTool;



    // TTree variables [all possible ones]
    // ************
    TTreeReaderValue<float> * m_weight_mc;
    TTreeReaderValue<float> * m_weight_pileup;
    TTreeReaderValue<float> * m_weight_btagSF;
    TTreeReaderValue<float> * m_weight_pileup_UP;
    TTreeReaderValue<float> * m_weight_pileup_DOWN;

    // Event info 
    TTreeReaderValue<int> * m_eventNumber;
    TTreeReaderValue<int> * m_runNumber;
    TTreeReaderValue<int> * m_lumiblock;
    TTreeReaderValue<float> * m_rho;
    TTreeReaderValue<int> * m_npv;
    TTreeReaderValue<int> * m_NGoodVtx;
    TTreeReaderValue<int> * m_LHAPDF_ID;
    TTreeReaderValue<int> * m_NIsoTrk;
    TTreeReaderValue<int> * m_true_pileup;
    TTreeReaderValue<float> * m_treeXSection;
    TTreeReaderValue<float> * m_treeKFactor;
    TTreeReaderValue<float> * m_treeSumOfWeights;

    TTreeReaderValue<float> * m_dnn_score;

    // MET
    TTreeReaderValue<float> * m_met_met;
    TTreeReaderValue<float> * m_met_phi;
    TTreeReaderValue<float> * m_met_met_sf;
    TTreeReaderValue<float> * m_met_phi_sf;

    // HT/ST
    TTreeReaderValue<float> * m_treeHT;
    TTreeReaderValue<float> * m_treeST;

    // Leptons
    TTreeReaderValue<std::vector<float>> * m_el_pt;
    TTreeReaderValue<std::vector<float>> * m_el_eta;
    TTreeReaderValue<std::vector<float>> * m_el_phi;
    TTreeReaderValue<std::vector<float>> * m_el_e;
    TTreeReaderValue<std::vector<float>> * m_el_charge;
    TTreeReaderValue<std::vector<int>> * m_el_iso;
    TTreeReaderValue<std::vector<int>> * m_el_veto;
    TTreeReaderValue<std::vector<int>> * m_el_loose;
    TTreeReaderValue<std::vector<int>> * m_el_medium;
    TTreeReaderValue<std::vector<int>> * m_el_tight;

    TTreeReaderValue<std::vector<float>> * m_mu_pt;
    TTreeReaderValue<std::vector<float>> * m_mu_eta;
    TTreeReaderValue<std::vector<float>> * m_mu_phi;
    TTreeReaderValue<std::vector<float>> * m_mu_e;
    TTreeReaderValue<std::vector<float>> * m_mu_charge;
    TTreeReaderValue<std::vector<int>> * m_mu_iso;
    TTreeReaderValue<std::vector<int>> * m_mu_loose;
    TTreeReaderValue<std::vector<int>> * m_mu_medium;
    TTreeReaderValue<std::vector<int>> * m_mu_medium2016;
    TTreeReaderValue<std::vector<int>> * m_mu_tight;
    TTreeReaderValue<std::vector<int>> * m_mu_hightPt;

    // large-R jet info
    TTreeReaderValue<float> * m_weight_ljet_jer;

    TTreeReaderValue<std::vector<float>> * m_ljet_pt;
    TTreeReaderValue<std::vector<float>> * m_ljet_eta;
    TTreeReaderValue<std::vector<float>> * m_ljet_phi;
    TTreeReaderValue<std::vector<float>> * m_ljet_e;
    TTreeReaderValue<std::vector<float>> * m_ljet_tau1;
    TTreeReaderValue<std::vector<float>> * m_ljet_tau2;
    TTreeReaderValue<std::vector<float>> * m_ljet_tau3;
    TTreeReaderValue<std::vector<float>> * m_ljet_tau21;
    TTreeReaderValue<std::vector<float>> * m_ljet_tau32;
    TTreeReaderValue<std::vector<float>> * m_ljet_charge;
    TTreeReaderValue<std::vector<float>> * m_ljet_SDmass;
    TTreeReaderValue<std::vector<float>> * m_ljet_CSVv2;
    TTreeReaderValue<std::vector<float>> * m_ljet_JERSF;
    TTreeReaderValue<std::vector<float>> * m_ljet_JERSFUp;
    TTreeReaderValue<std::vector<float>> * m_ljet_JERSFDown;
    TTreeReaderValue<std::vector<int>> * m_ljet_ID_loose;
    TTreeReaderValue<std::vector<int>> * m_ljet_ID_medium;
    TTreeReaderValue<std::vector<int>> * m_ljet_ID_tight;
    TTreeReaderValue<std::vector<int>> * m_ljet_ID_tightlepveto;

    TTreeReaderValue<std::vector<int>> * m_ljet_subjetIndex0;
    TTreeReaderValue<std::vector<int>> * m_ljet_subjetIndex1;
    TTreeReaderValue<std::vector<float>> * m_ljet_subjet_pt;
    TTreeReaderValue<std::vector<float>> * m_ljet_subjet_eta;
    TTreeReaderValue<std::vector<float>> * m_ljet_subjet_phi;
    TTreeReaderValue<std::vector<float>> * m_ljet_subjet_e;
    TTreeReaderValue<std::vector<float>> * m_ljet_subjet_charge;
    TTreeReaderValue<std::vector<float>> * m_ljet_subjet_CSVv2;
    TTreeReaderValue<std::vector<float>> * m_ljet_jec;
    TTreeReaderValue<std::vector<float>> * m_ljet_jer_up;
    TTreeReaderValue<std::vector<float>> * m_ljet_jer_down;

    // truth large-R jet info
    TTreeReaderValue<std::vector<float>> * m_truth_ljet_pt;
    TTreeReaderValue<std::vector<float>> * m_truth_ljet_eta;
    TTreeReaderValue<std::vector<float>> * m_truth_ljet_phi;
    TTreeReaderValue<std::vector<float>> * m_truth_ljet_e;
    TTreeReaderValue<std::vector<float>> * m_truth_ljet_charge;
    TTreeReaderValue<std::vector<float>> * m_truth_ljet_subjet_pt;
    TTreeReaderValue<std::vector<float>> * m_truth_ljet_subjet_eta;
    TTreeReaderValue<std::vector<float>> * m_truth_ljet_subjet_phi;
    TTreeReaderValue<std::vector<float>> * m_truth_ljet_subjet_e;
    TTreeReaderValue<std::vector<float>> * m_truth_ljet_subjet_charge;

    // Jet info
    TTreeReaderValue<float> * m_weight_jet_jer;

    TTreeReaderValue<std::vector<float>> * m_jet_pt;
    TTreeReaderValue<std::vector<float>> * m_jet_eta;
    TTreeReaderValue<std::vector<float>> * m_jet_phi;
    TTreeReaderValue<std::vector<float>> * m_jet_e;
    TTreeReaderValue<std::vector<float>> * m_jet_charge;
    TTreeReaderValue<std::vector<float>> * m_jet_CSVv2;
    TTreeReaderValue<std::vector<int>> * m_jet_ID_loose;
    TTreeReaderValue<std::vector<int>> * m_jet_ID_medium;
    TTreeReaderValue<std::vector<int>> * m_jet_ID_tight;
    TTreeReaderValue<std::vector<int>> * m_jet_ID_tightlepveto;
    TTreeReaderValue<std::vector<float>> * m_jet_jec;
    TTreeReaderValue<std::vector<float>> * m_jet_jer_up;
    TTreeReaderValue<std::vector<float>> * m_jet_jer_down;

    // truth jet info
    TTreeReaderValue<std::vector<float>> * m_truth_jet_pt;
    TTreeReaderValue<std::vector<float>> * m_truth_jet_eta;
    TTreeReaderValue<std::vector<float>> * m_truth_jet_phi;
    TTreeReaderValue<std::vector<float>> * m_truth_jet_e;
    TTreeReaderValue<std::vector<float>> * m_truth_jet_charge;

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
    TTreeReaderValue<std::vector<float>> * mc_pt;
    TTreeReaderValue<std::vector<float>> * mc_eta;
    TTreeReaderValue<std::vector<float>> * mc_phi;
    TTreeReaderValue<std::vector<float>> * mc_e;
    TTreeReaderValue<std::vector<float>> * mc_pdgId;
    TTreeReaderValue<std::vector<float>> * mc_charge;
    TTreeReaderValue<std::vector<int>> * mc_mom_idx;

    TTreeReaderValue<float> * m_mc_ht;                // evt_Gen_Ht

    // HLT 
    TTreeReaderValue<int> * m_HLT_Ele45_WPLoose_Gsf;          // HLT_Ele45_WPLoose_Gsf
    TTreeReaderValue<int> * m_HLT_Mu50;            // HLT_Mu50
    TTreeReaderValue<int> * m_HLT_TkMu50;          // HLT_TkMu50

    TTreeReaderValue<std::vector<float>> * m_mc_pt;
    TTreeReaderValue<std::vector<float>> * m_mc_eta;
    TTreeReaderValue<std::vector<float>> * m_mc_phi;
    TTreeReaderValue<std::vector<float>> * m_mc_e;
    TTreeReaderValue<std::vector<float>> * m_mc_charge;
    TTreeReaderValue<std::vector<float>> * m_mc_pdgId;
    TTreeReaderValue<std::vector<float>> * m_mc_mom_idx;
};

#endif
