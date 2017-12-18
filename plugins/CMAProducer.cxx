/*
Created:        --
Last Updated:   15 December 2017

Dan Marley
daniel.edison.marley@cernSPAMNOT.ch
University of Michigan, Ann Arbor, MI 48109

-----

Producer for CyMiniAna
 Runs in the EDM framework of CMSSW over EDMntuples
 Gather event information and put into a new object
*/
#include "Analysis/CyMiniAna/interface/CMAProducer.h"


// constructor
CMAProducer::CMAProducer(const edm::ParameterSet& iConfig) :
  m_isMC(iConfig.getParameter<bool>("isMC")),
  m_cleanFlags(iConfig.getParameter<bool>("cleanEvents")),
  m_useTruth(iConfig.getParameter<bool>("useTruth")),
  m_useJets(iConfig.getParameter<bool>("useJets")),
  m_useLargeRJets(iConfig.getParameter<bool>("useLargeRJets")),
  m_useLeptons(iConfig.getParameter<bool>("useLeptons")),
  m_useNeutrinos(iConfig.getParameter<bool>("useNeutrinos")),
  m_kinematicReco(iConfig.getParameter<bool>("kinematicReco")),
  m_metadataFile(iConfig.getParameter<std::string>("metadataFile")),
  m_LUMI(iConfig.getParameter<float>("LUMI")),

  // EDModules
  t_runno(consumes<int>(iConfig.getParameter<edm::InputTag>("runno"))),
  t_npv(consumes<int>(iConfig.getParameter<edm::InputTag>("npvLabel"))),
  t_rho(consumes<float>(iConfig.getParameter<edm::InputTag>("rhoLabel"))),

  // MET
  t_metFullPhi(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("metFullPhiLabel")),
  t_metFullPt(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("metFullPtLabel")),
  t_metFullPx(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("metFullPxLabel")),
  t_metFullPy(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("metFulluncorPhiLabel")),
  t_metFulluncorPhi(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("metFulluncorPhiLabel")),
  t_metFulluncorPt(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("metFulluncorPtLabel")),
  t_metFulluncorSumEt(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("metFulluncorSumEtLabel")){
    // Electrons and Muons
    if (m_useLeptons){
        m_electronsTool = new Electron(iConfig,consumesCollector());
        m_muonsTool     = new Muon(iConfig,consumesCollector());
    }

    // Neutrinos (reconstructed from MET)
    if (m_useNeutrinos && !m_kinematicReco){
        m_neutrinosTool = new Neutrino(iConfig,consumesCollector());
    }

    // AK4 Jets
    if (m_useJets){
        m_jetsTool = new Jets(iConfig,consumesCollector());
    }

    // AK8 Jets
    if (m_useLargeRJets){
        m_ljetsTool = new LargeRJets(iConfig,consumesCollector());
    }

    // Triggers
    if (m_useTriggers){
        m_triggersTool = new Trigger(iConfig,consumesCollector());
    }

    // Generator information (parton-level information)
    if (m_isMC){
    }

    // MET
    m_METPtMin = iConfig.getParameter<float>("METPtMin");

    // Register products (physics objects)
    produces<std::vector<Electron>>("electrons");
    produces<std::vector<Muon>>("muons");
    produces<std::vector<Jet>>("jets");
    produces<std::vector<Ljet>>("ljets");
    produces<std::vector<Neutrino>>("neutrinos");
    produces<MET>("MET");
    produces<float>("HT");
    produces<float>("ST");
    /* Add information from kinematics reconstruction once available */
} // end constructor


CMAProducer::~CMAProducer() {}


void CMAProducer::beginJob(){
    /* Producer setup before the event loop */
    m_electrons.clear();
    m_muons.clear();
    m_jets.clear();
    m_ljets.clear();
    m_neutrinos.clear();
    m_met.clear();
    m_HT = 0;
    m_ST = 0;

    return;
}


void CMAProducer::produce(edm::Event& evt, const edm::EventSetup& ){
    /* Get the values from the event 
       - Build the different physics objects
       - Store for easy access in other EDModules
    */
    edm::Handle<int> h_runno; 
    evt.getByToken(t_runno, h_runno);
    const int runno(*h_runno.product()) ; 

    // Event information

    // physics objects
    // - pass 'evt' to each function to access handles
    if (m_useLeptons){
        m_electrons = m_electronTool->initialize_electrons(evt);    // Electrons
        m_muons     = m_muonTool->initialize_muons(evt);        // Muons
    }
    if (m_useNeutrinos){
        m_neutrinos = m_neutrinoTool->initialize_neutrinos(evt);    // Neutrinos
    }
    if (m_useJets){
        m_jets = jetTool->initialize_jets(evt);         // AK4 jets
    }
    if (m_useLargeRJets){
        m_ljets = ljetTool->initialize_largeRjets(evt);   // AK8 jets
    }

    initialize_kinematics(evt);       // MET, HT calculations

    /* do reconstruction, weight calculations here */

    evt.put(m_electrons, "electrons");
    evt.put(m_muons,     "muons");
    evt.put(m_neutrinos, "neutrinos");
    evt.put(m_jets,      "jets");
    evt.put(m_ljets,     "ljets");
    evt.put(m_MET,       "MET");
    evt.put(m_HT,        "HT");
    evt.put(m_ST,        "ST");

    return;
}


void CMAProducer::endJob(){
    /* At the close */
    return;
}


void CMAProducer::initialize_kinematics(edm::Event& evt){
    /* Kinematic variables: MET, HT, and ST */
    // MET
    evt.getByLabel(t_metFullPhi, h_metFullPhi);
    evt.getByLabel(t_metFullPt,  h_metFullPt);
    evt.getByLabel(t_metFullPx,  h_metFullPx); // should be accessible using pT and phi
    evt.getByLabel(t_metFullPy,  h_metFullPy); // should be accessible using pT and phi
    evt.getByLabel(t_metFulluncorPhi,   h_metFulluncorPhi);
    evt.getByLabel(t_metFulluncorPt,    h_metFulluncorPt);
    evt.getByLabel(t_metFulluncorSumEt, h_metFulluncorSumEt);

    float metFullPt = (h_metFullPt.product())->at(0);

    m_MET = {};
    m_MET.p4.SetPtEtaPhiM( metFullPt, 0, h_metFullPhi.product()->at(0), 0);
    m_MET.uncorrPhi   = h_metFulluncorPt.product()->at(0);
    m_MET.uncorrPt    = h_metFulluncorPhi.product()->at(0);
    m_MET.uncorrSumEt = h_metFulluncorSumEt.product()->at(0);

    // HT & ST
    m_HT = 0;           // sum of jet pT
    m_ST = metFullPt;   // sum of jet pT, lepton pT, MET
    for (const auto& jet : m_jets){
        m_HT += jet.p4.Pt();
        m_ST += jet.p4.Pt();
    }
    for (const auto& el : m_electrons){
        m_ST += el.p4.Pt();
    }
    for (const auto& mu : m_muons){
        m_ST += mu.p4.Pt();
    }

    return;
}

// THE END
