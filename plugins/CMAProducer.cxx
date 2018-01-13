/*
Created:        15 December 2017
Last Updated:   12 January  2018

Dan Marley
daniel.edison.marley@cernSPAMNOT.ch
Texas A&M University

-----

Producer for CyMiniAna
 Runs in the EDM framework of CMSSW over EDMntuples
 Gather event information and put into a new object
*/
#include "Analysis/CyMiniAna/interface/CMAProducer.h"

using namespace edm;

// constructor
CMAProducer::CMAProducer(const edm::ParameterSet& iConfig) :
  m_isMC(iConfig.getParameter<bool>("isMC")),
  m_cleanFlags(iConfig.getParameter<bool>("cleanEvents")),
  m_useTruth(iConfig.getParameter<bool>("useTruth")),
  m_useJets(iConfig.getParameter<bool>("useJets")),
  m_useLargeRJets(iConfig.getParameter<bool>("useLargeRJets")),
  m_useLeptons(iConfig.getParameter<bool>("useLeptons")),
  m_useNeutrinos(iConfig.getParameter<bool>("useNeutrinos")),
  m_buildNeutrinos(iConfig.getParameter<bool>("buildNeutrinos")),
  m_kinematicReco(iConfig.getParameter<bool>("kinematicReco")),
  m_metadataFile(iConfig.getParameter<std::string>("metadataFile")),
  m_LUMI(iConfig.getParameter<bool>("LUMI")),
  // EDModules
  t_rho(consumes<float>(iConfig.getParameter<edm::InputTag>("rhoLabel"))),
  t_runno(consumes<int>(iConfig.getParameter<edm::InputTag>("runno"))),
  t_npv(consumes<unsigned int>(iConfig.getParameter<edm::InputTag>("npvLabel"))),
  // MET
  t_metFullPhi(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("metFullPhiLabel"))),
  t_metFullPt(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("metFullPtLabel"))),
  t_metFullPx(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("metFullPxLabel"))),
  t_metFullPy(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("metFulluncorPhiLabel"))),
  t_metFulluncorPhi(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("metFulluncorPhiLabel"))),
  t_metFulluncorPt(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("metFulluncorPtLabel"))),
  t_metFulluncorSumEt(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("metFulluncorSumEtLabel"))){
    m_objectSelectionTool = new objectSelection(iConfig);

    // Electrons and Muons
    m_electronsTool = new Electrons(iConfig,consumesCollector());
    m_muonsTool     = new Muons(iConfig,consumesCollector());

    // Neutrinos (reconstructed from MET)
    m_neutrinosTool = new Neutrinos(iConfig,consumesCollector());

    // AK4 Jets
    m_jetsTool = new Jets(iConfig,consumesCollector());

    // AK8 Jets
    m_ljetsTool = new LargeRJets(iConfig,consumesCollector());

    // Triggers
    //m_triggersTool = new Trigger(iConfig,consumesCollector());

    // Generator information (parton-level information)
    if (m_isMC){
    }

    // MET
    m_METPtMin = iConfig.getParameter<double>("METPtMin");

    // Register products (physics objects)
    produces<std::vector<Electron>>("electrons").setBranchAlias( "electrons" );
    produces<std::vector<Muon>>("muons").setBranchAlias( "muons" );
    produces<std::vector<Jet>>("jets").setBranchAlias( "jets" );
    produces<std::vector<Ljet>>("ljets").setBranchAlias( "ljets" );
    produces<std::vector<Neutrino>>("neutrinos").setBranchAlias( "neutrinos" );
    produces<MET>("MET").setBranchAlias( "MET" );
    produces<float>("HT").setBranchAlias( "HT" );
    produces<float>("ST").setBranchAlias( "ST" );
    /* Add information from kinematics reconstruction once available */
} // end constructor


CMAProducer::~CMAProducer() {
    // Clean-up
    delete m_electronsTool;
    delete m_muonsTool;
    delete m_neutrinosTool;
    delete m_jetsTool;
    delete m_ljetsTool;
    //delete m_triggersTool;
}


void CMAProducer::beginJob(const edm::EventSetup&){
    /* Producer setup before the event loop */
    m_objectSelectionTool->initialize();

    m_electrons.clear();
    m_muons.clear();
    m_leptons.clear();    // contain electrons and muons
    m_jets.clear();
    m_ljets.clear();
    m_neutrinos.clear();
    m_MET = {};
    m_HT = 0;
    m_ST = 0;

    return;
}


void CMAProducer::produce(edm::Event& evt, const edm::EventSetup& ){
    /* Get the values from the event 
       - Build the different physics objects
       - Store for easy access in other EDModules
    */
    // Setup objects for output
    std::auto_ptr< std::vector<Electron> > e_electrons( new std::vector<Electron> );
    std::auto_ptr< std::vector<Muon> > e_muons( new std::vector<Muon> );
    std::auto_ptr< std::vector<Neutrino> > e_neutrinos( new std::vector<Neutrino> );
    std::auto_ptr< std::vector<Jet> > e_jets( new std::vector<Jet> );
    std::auto_ptr< std::vector<Ljet> > e_ljets( new std::vector<Ljet> );
    std::auto_ptr< MET > e_MET( new MET );
    std::auto_ptr< float > e_HT( new float );
    std::auto_ptr< float > e_ST( new float );


    // Event information
//    edm::Handle<int> h_runno;
//    evt.getByToken(t_runno, h_runno);
//    const int runno(*h_runno.product());


    // Build physics objects
    // - pass 'evt' to each function to access handles
    if (m_useLeptons){
        m_electrons = m_electronsTool->execute(evt,*m_objectSelectionTool); // Electrons
        m_muons     = m_muonsTool->execute(evt,*m_objectSelectionTool);     // Muons

        unsigned int nEls = m_electrons.size();
        unsigned int nMus = m_muons.size();
        m_leptons.resize( nEls+nMus );
        for (unsigned int el=0;el<nEls;el++){
            m_leptons.at(el) = m_electrons.at(el);
            e_electrons->push_back( m_electrons.at(el) );
        }
        for (unsigned int mu=0;mu<nMus;mu++){ 
            m_leptons.at(mu+nEls) = m_muons.at(mu);
            e_muons->push_back( m_muons.at(mu) );
        }
    }

    if (m_useJets){
        m_jets = m_jetsTool->execute(evt,*m_objectSelectionTool);             // AK4 jets
        for (auto& jet : m_jets)
            e_jets->push_back(jet);
    }

    if (m_useLargeRJets){
        m_ljets = m_ljetsTool->execute(evt,*m_objectSelectionTool);           // AK8 jets
        for (auto& jet : m_ljets)
            e_ljets->push_back(jet);
    }

    initialize_kinematics(evt);                                           // MET, HT

    if (m_useNeutrinos){
        // Neutrinos (reconstruct or grab from ntuple)
        if (m_buildNeutrinos)
            m_neutrinos = m_neutrinosTool->execute(m_leptons.at(0),m_MET,*m_objectSelectionTool);
        else
            m_neutrinos = m_neutrinosTool->execute(evt,*m_objectSelectionTool);

        for (auto& nu : m_neutrinos)
            e_neutrinos->push_back(nu);
    }

    /* 
     * do kinematic reconstruction, event weight calculations here 
     */

    (*e_MET).p4 = m_MET.p4;
    (*e_MET).uncorrPt    = m_MET.uncorrPt;
    (*e_MET).uncorrPhi   = m_MET.uncorrPhi;
    (*e_MET).uncorrSumEt = m_MET.uncorrSumEt;

    *e_HT = m_HT;
    *e_ST = m_ST;

    evt.put(e_electrons, "electrons");
    evt.put(e_muons,     "muons");
    evt.put(e_neutrinos, "neutrinos");
    evt.put(e_jets,      "jets");
    evt.put(e_ljets,     "ljets");
    evt.put(e_MET,       "MET");
    evt.put(e_HT,        "HT");
    evt.put(e_ST,        "ST");

    return;
}


void CMAProducer::endJob(){
    /* At the close */
    return;
}


void CMAProducer::initialize_kinematics(edm::Event& evt){
    /* Kinematic variables: MET, HT, and ST */
    // MET
    evt.getByToken(t_metFullPhi, h_metFullPhi);
    evt.getByToken(t_metFullPt,  h_metFullPt);
    evt.getByToken(t_metFullPx,  h_metFullPx); // should be accessible using pT and phi
    evt.getByToken(t_metFullPy,  h_metFullPy); // should be accessible using pT and phi
    evt.getByToken(t_metFulluncorPhi,   h_metFulluncorPhi);
    evt.getByToken(t_metFulluncorPt,    h_metFulluncorPt);
    evt.getByToken(t_metFulluncorSumEt, h_metFulluncorSumEt);

    float metFullPt = (h_metFullPt.product())->at(0);

    m_MET = {};
    m_MET.p4.SetPtEtaPhiM( metFullPt, 0, h_metFullPhi.product()->at(0), 0);
    m_MET.uncorrPt    = h_metFulluncorPt.product()->at(0);
    m_MET.uncorrPhi   = h_metFulluncorPhi.product()->at(0);
    m_MET.uncorrSumEt = h_metFulluncorSumEt.product()->at(0);

    // HT & ST
    m_HT = 0;           // sum of jet pT
    m_ST = metFullPt;   // sum of jet pT, lepton pT, MET
    for (const auto& jet : m_jets){
        m_HT += jet.p4.Pt();
        m_ST += jet.p4.Pt();
    }

    for (const auto& el : m_electrons)
        m_ST += el.p4.Pt();

    for (const auto& mu : m_muons)
        m_ST += mu.p4.Pt();

    return;
}

DEFINE_FWK_MODULE(CMAProducer);
// THE END
