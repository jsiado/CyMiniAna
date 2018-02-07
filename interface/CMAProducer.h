#ifndef CMAPRODUCER_H
#define CMAPRODUCER_H

#include <set>
#include <list>
#include <memory>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TSystem.h"
#include "TLorentzVector.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

#include "Analysis/CyMiniAna/interface/tools.h"
#include "Analysis/CyMiniAna/interface/physicsObjects.h"
#include "Analysis/CyMiniAna/interface/objectSelection.h"
#include "Analysis/CyMiniAna/interface/Jets.h"
#include "Analysis/CyMiniAna/interface/LargeRJets.h"
#include "Analysis/CyMiniAna/interface/Electrons.h"
#include "Analysis/CyMiniAna/interface/Muons.h"
#include "Analysis/CyMiniAna/interface/ETMiss.h"
#include "Analysis/CyMiniAna/interface/Neutrinos.h"


// CMAProducer Class
class CMAProducer : public edm::EDProducer {
  public:
    // Constructor
    explicit CMAProducer(const edm::ParameterSet& iConfig);

    // Destructor
    virtual ~CMAProducer();

    static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

  private:

    virtual void beginJob(const edm::EventSetup&);
    virtual void produce(edm::Event&, const edm::EventSetup&);
    virtual void endJob();

    void clearObjects();

    // Build physics objects
    void initialize_electrons(edm::Event& evt);
    void initialize_muons(edm::Event& evt);
    void initialize_neutrinos(edm::Event& evt);
    void initialize_jets(edm::Event& evt);
    void initialize_largeRjets(edm::Event& evt);
    void initialize_kinematics(edm::Event& evt);

    Electrons m_electronsTool;
    Muons m_muonsTool;
    Neutrinos m_neutrinosTool;
    Jets m_jetsTool;
    LargeRJets m_ljetsTool;
    ETMiss m_METTool;
    //Trigger m_triggersTool;
    objectSelection m_objectSelectionTool;

    // General Parameters
    bool m_isMC;
    bool m_cleanFlags;
    bool m_useTruth;
    bool m_useJets;
    bool m_useLargeRJets;
    bool m_useLeptons;
    bool m_useNeutrinos;
    bool m_buildNeutrinos;
    bool m_kinematicReco;
    double m_LUMI;

    // Physics information
    std::vector<Electron> m_electrons;
    std::vector<Muon> m_muons;
    std::vector<Muon> m_truth_muons;
    std::vector<Lepton> m_leptons;
    std::vector<Neutrino> m_neutrinos;
    std::vector<Jet> m_jets;
    std::vector<Jet> m_truth_jets;
    std::vector<Ljet> m_ljets;
    std::vector<Ljet> m_truth_ljets;
    MET m_MET;
    double m_HT;
    double m_ST;
    double m_event_weight;

    std::string m_metadataFile;
    std::string m_sampleName;                      // unique name for the sample
    std::map<std::string, float> m_XSection;	   // map sample name to XSection
    std::map<std::string, float> m_KFactor;        // map sample name to KFactor
    std::map<std::string, float> m_sumOfWeights;   // map sample name to sum of weights
    std::map<std::string, unsigned int> m_NEvents; // map sample name to number of events

    // ************
    // Setup to read EDMntuple format
    // -- following this example:
    //    https://github.com/dmajumder/VLQAna
    // ----------member data ---------------------------
    //edm::ConsumesCollector& m_iC; // = consumesCollector();
    edm::InputTag m_src;

    // Tokens
    edm::EDGetTokenT<int> t_evtno;
    edm::EDGetTokenT<bool> t_isData;
    edm::EDGetTokenT<bool> t_hltdecision;
    edm::EDGetTokenT<std::string> t_evttype;
    edm::EDGetTokenT<float> t_evtwtGen;
    edm::EDGetTokenT<float> t_evtwtPV;
    edm::EDGetTokenT<float> t_evtwtPVBG;
    edm::EDGetTokenT<float> t_evtwtPVH;
    edm::EDGetTokenT<float> t_evtwtPVLow;
    edm::EDGetTokenT<float> t_evtwtPVHigh;
    edm::EDGetTokenT<unsigned int> t_npv;
    edm::EDGetTokenT<int> t_npuTrue;
    edm::EDGetTokenT<float> t_htHat;
    edm::EDGetTokenT<std::vector<int>> t_lhewtids;
    edm::EDGetTokenT<std::vector<float>> t_lhewts;
    edm::EDGetTokenT<GenEventInfoProduct> t_genEvtInfoProd;

    // Handles
    edm::Handle<int> h_evtno;
    edm::Handle<bool> h_isData;
    edm::Handle<bool> h_hltdecision;
    edm::Handle<std::string> h_evttype;
    edm::Handle<float> h_evtwtGen;
    edm::Handle<float> h_evtwtPV;
    edm::Handle<float> h_evtwtPVBG;
    edm::Handle<float> h_evtwtPVH;
    edm::Handle<float> h_evtwtPVLow;
    edm::Handle<float> h_evtwtPVHigh;
    edm::Handle<unsigned int> h_npv;
    edm::Handle<int> h_npuTrue;
    edm::Handle<float> h_htHat;
    edm::Handle<std::vector<int>> h_lhewtids;
    edm::Handle<std::vector<float>> h_lhewts;
    edm::Handle<GenEventInfoProduct> h_genEvtInfoProd;
};

#endif
