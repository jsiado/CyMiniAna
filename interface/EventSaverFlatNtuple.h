#ifndef EVENTSAVERFLATNTUPLE_H
#define EVENTSAVERFLATNTUPLE_H

#include <string>
#include <map>
#include <vector>
#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/one/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Utilities/interface/EDMException.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/Provenance/interface/EventAuxiliary.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TSystem.h"

#include "Analysis/CyMiniAna/interface/tools.h"
#include "Analysis/CyMiniAna/interface/physicsObjects.h"
#include "Analysis/CyMiniAna/interface/Truth.h"

class EventSaverFlatNtuple : public edm::one::EDAnalyzer<edm::one::SharedResources> {
//class EventSaverFlatNtuple : public edm::EDProducer {
  public:

    EventSaverFlatNtuple( const edm::ParameterSet & );
    virtual ~EventSaverFlatNtuple();

    void initialize_branches();

  private:

    virtual void beginJob() override;
    virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
    virtual void endJob() override;

    TTree* m_ttree;                // physics information
    TTree* m_metadata_ttree;       // metadata

    std::string m_name;
    bool m_isMC;
    bool m_useSystWeights;
    bool m_putOverflowInLastBin;
    bool m_putUnderflowInFirstBin;

    bool m_useJets;
    bool m_useLargeRJets;
    bool m_useLeptons;
    bool m_useNeutrinos;

    std::string t_sampleName;
    std::string t_metadataFile;

    std::map<std::string,Sample> m_mapOfSamples;     // map of Sample structs
    std::map<std::string, float> m_XSections;        // map sample name to XSection
    std::map<std::string, float> m_KFactors;         // map sample name to KFactor
    std::map<std::string, float> m_sumOfMCWeights;   // map sample name to sum of weights
    std::map<std::string, unsigned int> m_NEvents;   // map sample name to number of events in sample

    Truth m_truthTool;

    // Handles and Tokens
    edm::EDGetTokenT<std::vector<Electron>> t_electrons;
    edm::EDGetTokenT<std::vector<Muon>> t_muons;
    edm::EDGetTokenT<std::vector<Neutrino>> t_neutrinos;
    edm::EDGetTokenT<std::vector<Jet>> t_jets;
    edm::EDGetTokenT<std::vector<Jet>> t_truth_jets;
    edm::EDGetTokenT<std::vector<Ljet>> t_ljets;
    edm::EDGetTokenT<std::vector<Ljet>> t_truth_ljets;
    edm::EDGetTokenT<MET> t_met;
    edm::EDGetTokenT<double> t_HT;
    edm::EDGetTokenT<double> t_ST;
    edm::EDGetTokenT<std::vector<float>> t_rho;
    edm::EDGetTokenT<int> t_runno;
    edm::EDGetTokenT<int> t_npv;
    edm::EDGetTokenT<int> t_npuTrue;
    edm::EDGetTokenT<int> t_evtno;
    edm::EDGetTokenT<int> t_lumisec;

    edm::EDGetTokenT<bool> t_isData;
    edm::EDGetTokenT<bool> t_hltdecision;
    edm::EDGetTokenT<std::string> t_evttype;
    edm::EDGetTokenT<float> t_evtwtGen;
    edm::EDGetTokenT<float> t_evtwtPV;
    edm::EDGetTokenT<float> t_evtwtPVBG;
    edm::EDGetTokenT<float> t_evtwtPVH;
    edm::EDGetTokenT<float> t_evtwtPVLow;
    edm::EDGetTokenT<float> t_evtwtPVHigh;
    edm::EDGetTokenT<float> t_htHat;
    edm::EDGetTokenT<std::vector<int>> t_lhewtids;
    edm::EDGetTokenT<std::vector<float>> t_lhewts;
    edm::EDGetTokenT<GenEventInfoProduct> t_genEvtInfoProd;

    // Handles
    edm::Handle<bool> h_isData;
    edm::Handle<bool> h_hltdecision;
    edm::Handle<std::string> h_evttype;
    edm::Handle<float> h_evtwtGen;
    edm::Handle<float> h_evtwtPV;
    edm::Handle<float> h_evtwtPVBG;
    edm::Handle<float> h_evtwtPVH;
    edm::Handle<float> h_evtwtPVLow;
    edm::Handle<float> h_evtwtPVHigh;
    edm::Handle<float> h_htHat;
    edm::Handle<std::vector<int>> h_lhewtids;
    edm::Handle<std::vector<float>> h_lhewts;

    edm::Handle<std::vector<Electron>> m_electrons;
    edm::Handle<std::vector<Muon>> m_muons;
    edm::Handle<std::vector<Neutrino>> m_neutrinos;
    edm::Handle<std::vector<Jet>> m_jets;
    edm::Handle<std::vector<Jet>> m_truth_jets;
    edm::Handle<std::vector<Ljet>> m_ljets;
    edm::Handle<std::vector<Ljet>> m_truth_ljets;
    edm::Handle<MET> m_met;
    edm::Handle<double> m_HT;
    edm::Handle<double> m_ST;
    edm::Handle<std::vector<float>> h_rho;
    edm::Handle<int> h_runno;
    edm::Handle<int> h_npv;
    edm::Handle<int> h_npuTrue;
    edm::Handle<int> h_evtno;
    edm::Handle<int> h_lumisec;
    edm::Handle<GenEventInfoProduct> h_genEvtInfoProd;

    // Branches
    std::string m_sampleName;
    float m_xsection;
    float m_kfactor;
    float m_sumOfWeights;

    std::vector<float> m_jet_pt;
    std::vector<float> m_jet_eta;
    std::vector<float> m_jet_phi;
    std::vector<float> m_jet_e;
    std::vector<float> m_jet_CSV;
    std::vector<float> m_jet_charge;
    std::vector<int> m_jet_ID_loose;
    std::vector<int> m_jet_ID_medium;
    std::vector<int> m_jet_ID_tight;
    std::vector<int> m_jet_ID_tightlepveto;
    std::vector<int> m_jet_true_flavor;

    std::vector<float> m_ljet_pt;
    std::vector<float> m_ljet_eta;
    std::vector<float> m_ljet_phi;
    std::vector<float> m_ljet_e;
    std::vector<float> m_ljet_CSV;
    std::vector<float> m_ljet_tau1;
    std::vector<float> m_ljet_tau2;
    std::vector<float> m_ljet_tau3;
    std::vector<float> m_ljet_tau21;
    std::vector<float> m_ljet_tau32;
    std::vector<float> m_ljet_charge;
    std::vector<float> m_ljet_SDmass;
    std::vector<int> m_ljet_ID_loose;
    std::vector<int> m_ljet_ID_medium;
    std::vector<int> m_ljet_ID_tight;
    std::vector<int> m_ljet_ID_tightlepveto;
    std::vector<int> m_ljet_subjetIndex0;
    std::vector<int> m_ljet_subjetIndex1;
    std::vector<float> m_ljet_subjet_pt;
    std::vector<float> m_ljet_subjet_eta;
    std::vector<float> m_ljet_subjet_phi;
    std::vector<float> m_ljet_subjet_e;
    std::vector<float> m_ljet_subjet_CSV;
    std::vector<float> m_ljet_subjet_charge;

    std::vector<float> m_el_pt;
    std::vector<float> m_el_eta;
    std::vector<float> m_el_phi;
    std::vector<float> m_el_e;
    std::vector<int> m_el_iso;
    std::vector<float> m_el_charge;
    std::vector<int> m_el_ID_loose;
    std::vector<int> m_el_ID_medium;
    std::vector<int> m_el_ID_tight;
    std::vector<int> m_el_reco;
    std::vector<float> m_el_SF_ID;
    std::vector<float> m_el_SF_reco;
    std::vector<float> m_el_SF_ID_UP;
    std::vector<float> m_el_SF_reco_UP;
    std::vector<float> m_el_SF_ID_DN;
    std::vector<float> m_el_SF_reco_DN;

    std::vector<float> m_mu_pt;
    std::vector<float> m_mu_eta;
    std::vector<float> m_mu_phi;
    std::vector<float> m_mu_e;
    std::vector<int> m_mu_iso;
    std::vector<float> m_mu_charge;
    std::vector<int> m_mu_ID_loose;
    std::vector<int> m_mu_ID_medium;
    std::vector<int> m_mu_ID_tight;
    std::vector<float> m_mu_SF_ID;
    std::vector<float> m_mu_SF_ISO;
    std::vector<float> m_mu_SF_trigger;
    std::vector<float> m_mu_SF_track;
    std::vector<float> m_mu_SF_ID_UP;
    std::vector<float> m_mu_SF_ISO_UP;
    std::vector<float> m_mu_SF_trigger_UP;
    std::vector<float> m_mu_SF_track_UP;
    std::vector<float> m_mu_SF_ID_DN;
    std::vector<float> m_mu_SF_ISO_DN;
    std::vector<float> m_mu_SF_trigger_DN;
    std::vector<float> m_mu_SF_track_DN;

    std::vector<float> m_nu_pt;
    std::vector<float> m_nu_eta;
    std::vector<float> m_nu_phi;
    std::vector<float> m_nu_e;

    float m_met_met;
    float m_met_phi;
    float m_HT_branch;
    float m_ST_branch;

    int m_runNumber;
    int m_eventNumber;
    int m_lumiblock;
    std::vector<float> m_rho;
    int m_nGoodVtx;
    int m_LHA_PDF_ID;
    int m_nIsoTrk;
    int m_true_pileup;
    unsigned int m_npv;

    float m_weight_mc;
    float m_weight_btag;
    float m_weight_pileup;
    float m_weight_jet_jer;
    float m_weight_ljet_jer;

    float m_met_met_sf;
    float m_met_phi_sf;
    std::vector<float> m_jet_jec;
    std::vector<float> m_jet_jer_up;
    std::vector<float> m_jet_jer_down;
    std::vector<float> m_ljet_jec;
    std::vector<float> m_ljet_jer_up;
    std::vector<float> m_ljet_jer_down;

    std::vector<float> m_mc_pt;
    std::vector<float> m_mc_eta;
    std::vector<float> m_mc_phi;
    std::vector<float> m_mc_e;
    std::vector<float> m_mc_pdgId;
    std::vector<float> m_mc_charge;
    std::vector<int> m_mc_parent0_idx;
    std::vector<int> m_mc_parent1_idx;
    std::vector<int> m_mc_child0_idx;
    std::vector<int> m_mc_child1_idx;

    std::vector<float> m_truth_jet_pt;
    std::vector<float> m_truth_jet_eta;
    std::vector<float> m_truth_jet_phi;
    std::vector<float> m_truth_jet_e;
    std::vector<float> m_truth_jet_charge;
    std::vector<float> m_truth_ljet_pt;
    std::vector<float> m_truth_ljet_eta;
    std::vector<float> m_truth_ljet_phi;
    std::vector<float> m_truth_ljet_e;
    std::vector<float> m_truth_ljet_charge;
};

#endif
