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
  // PhysicsObjects
  m_electronsTool(iConfig,consumesCollector()),
  m_muonsTool(iConfig,consumesCollector()),
  m_neutrinosTool(iConfig,consumesCollector()),
  m_jetsTool(iConfig,consumesCollector()),
  m_ljetsTool(iConfig,consumesCollector()),
  m_METTool(iConfig,consumesCollector()),
  m_objectSelectionTool(iConfig.getParameter<edm::ParameterSet>("objSelectionParams")),
  //m_triggersTool(iConfig,consumesCollector()){
  // General Parameters
  m_isMC(iConfig.getParameter<bool>("isMC")),
  m_useTruth(iConfig.getParameter<bool>("useTruth")),
  m_useJets(iConfig.getParameter<bool>("useJets")),
  m_useLargeRJets(iConfig.getParameter<bool>("useLargeRJets")),
  m_useLeptons(iConfig.getParameter<bool>("useLeptons")),
  m_useNeutrinos(iConfig.getParameter<bool>("useNeutrinos")),
  m_buildNeutrinos(iConfig.getParameter<bool>("buildNeutrinos")),
  m_kinematicReco(iConfig.getParameter<bool>("kinematicReco")),
  m_LUMI(iConfig.getParameter<double>("LUMI")),
  m_metadataFile(iConfig.getParameter<std::string>("metadataFile")),
  m_sampleName(iConfig.getParameter<std::string>("sampleName")){
    // Generator information (parton-level information)
    m_XSection.clear();
    m_KFactor.clear();
    m_sumOfWeights.clear();
    m_NEvents.clear();
    if (m_isMC){
        cma::getSampleWeights( m_metadataFile,m_XSection,m_KFactor,m_sumOfWeights,m_NEvents );
        t_genEvtInfoProd = consumes<GenEventInfoProduct>(iConfig.getParameter<std::string>("genEvtInfoProdName"));  // generator info
    }

    // Register products (physics objects & information)
    produces<std::vector<Electron>>("electrons").setBranchAlias( "electrons" );
    produces<std::vector<Muon>>("muons").setBranchAlias( "muons" );
    produces<std::vector<Jet>>("jets").setBranchAlias( "jets" );
    produces<std::vector<Jet>>("truthjets").setBranchAlias( "truthjets" );
    produces<std::vector<Ljet>>("ljets").setBranchAlias( "ljets" );
    produces<std::vector<Ljet>>("truthljets").setBranchAlias( "truthljets" );
    produces<std::vector<Neutrino>>("neutrinos").setBranchAlias( "neutrinos" );
    produces<MET>("MET").setBranchAlias( "MET" );
    produces<double>("HT").setBranchAlias( "HT" );
    produces<double>("ST").setBranchAlias( "ST" );
    produces<double>("eventweight").setBranchAlias( "eventweight" );

    /* Add information from kinematic reconstruction once available */
} // end constructor


CMAProducer::~CMAProducer() {
    // Clean-up
/*
    delete m_METTool;
    delete m_electronsTool;
    delete m_muonsTool;
    delete m_neutrinosTool;
    delete m_jetsTool;
    delete m_ljetsTool;
    delete m_triggersTool;
*/
}


void CMAProducer::beginJob(const edm::EventSetup&){
    /* Producer setup before the event loop */
    m_objectSelectionTool.initialize();
    clearObjects();
    return;
}


void CMAProducer::clearObjects(){
    /* Clear physics objects for the new event */
    m_electrons.clear();
    m_muons.clear();
    m_leptons.clear();    // contain electrons and muons
    m_jets.clear();
    m_ljets.clear();
    m_neutrinos.clear();
    m_MET = {};
    m_HT = 0;
    m_ST = 0;
    m_event_weight = 1;   // nominal event weight for histograms

    return;
}


void CMAProducer::produce(edm::Event& evt, const edm::EventSetup& ){
    /* Get the values from the event 
       - Build the different physics objects
       - Store for easy access in other EDModules
    */
    clearObjects();

    // Setup objects for output
    // Requires creating src/classes_def.xml & src/classes.h
    // https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideCreatingNewProducts
    std::auto_ptr< std::vector<Electron> > e_electrons( new std::vector<Electron> );
    std::auto_ptr< std::vector<Muon> > e_muons( new std::vector<Muon> );
    std::auto_ptr< std::vector<Neutrino> > e_neutrinos( new std::vector<Neutrino> );
    std::auto_ptr< std::vector<Jet> > e_jets( new std::vector<Jet> );
    std::auto_ptr< std::vector<Jet> > e_truth_jets( new std::vector<Jet> );
    std::auto_ptr< std::vector<Ljet> > e_ljets( new std::vector<Ljet> );
    std::auto_ptr< std::vector<Ljet> > e_truth_ljets( new std::vector<Ljet> );
    std::auto_ptr< MET > e_MET( new MET );
    std::auto_ptr< double > e_HT( new double );
    std::auto_ptr< double > e_ST( new double );
    std::auto_ptr< double > e_event_weight( new double );

    // Build physics objects
    // - pass 'evt' to each function to access handles
    if (m_useLeptons){
        m_electrons = m_electronsTool.execute(evt,m_objectSelectionTool); // Electrons
        m_muons     = m_muonsTool.execute(evt,m_objectSelectionTool);     // Muons

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

    m_jets.clear();
    m_truth_jets.clear();
    if (m_useJets){
        m_jets = m_jetsTool.execute(evt,m_objectSelectionTool);             // AK4 jets
        if (m_isMC) m_truth_jets = m_jetsTool.execute_truth(evt,m_objectSelectionTool);  // AK4 jets

        edm::LogInfo("Jets built "+std::to_string(m_jets.size()));
        for (auto& jet : m_jets)
            e_jets->push_back(jet);
        for (auto& jet : m_truth_jets)
            e_truth_jets->push_back(jet);
    }

    m_ljets.clear();
    m_truth_ljets.clear();
    if (m_useLargeRJets){
        m_ljets = m_ljetsTool.execute(evt,m_objectSelectionTool);           // AK8 jets
        if (m_isMC) m_truth_ljets = m_ljetsTool.execute_truth(evt,m_objectSelectionTool); // AK8 jets
        edm::LogInfo("LJets built "+std::to_string(m_jets.size()));

        for (auto& jet : m_ljets)
            e_ljets->push_back(jet);
        for (auto& jet : m_truth_ljets)
            e_truth_ljets->push_back(jet);
    }

    initialize_kinematics(evt);                                             // MET, HT

    if (m_useNeutrinos){
        // Neutrinos (reconstruct or grab from ntuple)
        if (m_buildNeutrinos)
            m_neutrinos = m_neutrinosTool.execute(m_leptons.at(0),m_MET,m_objectSelectionTool);
        else
            m_neutrinos = m_neutrinosTool.execute(evt,m_objectSelectionTool);

        for (auto& nu : m_neutrinos)
            e_neutrinos->push_back(nu);
    }

    /* 
     * do kinematic reconstruction, event weight calculations here 
     *
     *  - No btag SF yet
     *  - No pileup weight yet
     */
    // MC weight
    evt.getByToken(t_genEvtInfoProd,h_genEvtInfoProd);
    float mc_weight = h_genEvtInfoProd->weight();

    m_event_weight  = 1.0;
    m_event_weight *= m_XSection.at(m_sampleName) * m_KFactor.at(m_sampleName) * mc_weight / m_sumOfWeights.at(m_sampleName);
    m_event_weight /= m_LUMI;

    /* Set values to store in the event */
    (*e_MET).p4 = m_MET.p4;
    (*e_MET).uncorrPt    = m_MET.uncorrPt;
    (*e_MET).uncorrPhi   = m_MET.uncorrPhi;
    (*e_MET).uncorrSumEt = m_MET.uncorrSumEt;

    *e_HT = m_HT;
    *e_ST = m_ST;

    *e_event_weight = m_event_weight;

    /* Store values in the event to use in analyzers/filters */
    evt.put(e_electrons,  "electrons");
    evt.put(e_muons,      "muons");
    evt.put(e_neutrinos,  "neutrinos");
    evt.put(e_jets,       "jets");
    evt.put(e_truth_jets, "truthjets");
    evt.put(e_ljets,      "ljets");
    evt.put(e_truth_ljets,"truthljets");
    evt.put(e_MET,        "MET");
    evt.put(e_HT,         "HT");
    evt.put(e_ST,         "ST");
    evt.put(e_event_weight, "eventweight");

    return;
}


void CMAProducer::endJob(){
    /* At the close */
    return;
}


void CMAProducer::initialize_kinematics(edm::Event& evt){
    /* Kinematic variables: MET, HT, and ST */
    // MET
    m_MET = m_METTool.execute(evt,m_objectSelectionTool);

    // HT & ST
    m_HT = 0;               // sum of jet pT
    m_ST = m_MET.p4.Pt();   // sum of jet pT, lepton pT, MET
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


void CMAProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
    /* Method fills 'descriptions' with the allowed parameters for the module
       The following says we do not know what parameters are allowed so do no validation
       Please change this to state exactly what you do use, even if it is no parameters
    */
    edm::ParameterSetDescription desc;
    desc.setUnknown();
    descriptions.addDefault(desc);

    return;
}


DEFINE_FWK_MODULE(CMAProducer);
// THE END
