/*
Created:        --
Last Updated:   13 December 2017

Dan Marley
daniel.edison.marley@cernSPAMNOT.ch
University of Michigan, Ann Arbor, MI 48109

-----

Producer for CyMiniAna
 Runs in the EDM framework of CMSSW over EDMntuples
 Gather event information and put into cmaEvent object
*/
#include "Analysis/CyMiniAna/interface/Event.h"

// constructor
CMAProducer::CMAProducer(const edm::ParameterSet& iConfig) :
  m_isMC(iConfig.getParameter<bool>("isMC")),
  m_LUMI(iConfig.getParameter<bool>("LUMI")),
  m_cleanFlags(iConfig.getParameter<bool>("cleanEvents")),
  m_useTruth(iConfig.getParameter<bool>("useTruth")),
  m_useJets(iConfig.getParameter<bool>("useJets")),
  m_useLargeRJets(iConfig.getParameter<bool>("useLargeRJets")),
  m_useLeptons(iConfig.getParameter<bool>("useLeptons")),
  m_useNeutrino(iConfig.getParameter<bool>("useNeutrino")),
  m_kinematicReco(iConfig.getParameter<bool>("kinematicReco")),
  m_metadataFile(iConfig.getParameter<bool>("metadataFile")){
    // EDModules
    t_runno = consumes<int>(iConfig.getParameter<edm::InputTag>("runno");
    t_npv   = consumes<int>(iConfig.getParameter<edm::InputTag>("npvLabel");

    if (m_useLeptons){
        m_elPtMin     = iConfig.getParameter<double>("elPtMin");
        m_elAbsEtaMax = iConfig.getParameter<double>("elAbsEtaMax");
        m_applyIso    = iConfig.getParameter<bool>("applyIso");

        t_elCharge      = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("elChargeLabel"));
        t_elDxy         = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("elDxyLabel"));
        t_elDz          = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("elDzLabel"));
        t_elE           = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("elELabel"));
        t_elEta         = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("elEtaLabel"));
        t_elHoE         = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("elHoELabel"));
        t_elRelIsoEA    = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("elRelIsoEALabel"));
        t_elKey         = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("elKeyLabel"));
        t_elPhi         = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("elPhiLabel"));
        t_elPt          = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("elPtLabel"));
        t_eldEtaIn      = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("eldEtaInLabel"));
        t_eldEtaInSeed  = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("eldEtaInSeedLabel"));
        t_eldPhiIn      = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("eldPhiInLabel"));
        t_elfull5x5siee = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("elfull5x5sieeLabel"));
        t_elvidLoose    = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("elvidLooseLabel"));
        t_elvidMedium   = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("elvidMediumLabel"));
        t_elvidTight    = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("elvidTightLabel"));
        t_elvidVeto     = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("elvidVetoLabel"));
        t_elvidHEEP     = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("elvidHEEPLabel"));
        t_elmissHits    = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("elmissHitsLabel"));
        t_elooEmooP     = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("elooEmooPLabel"));
        t_elscEta       = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("elscEtaLabel"));
        t_elhasMatchedConVeto = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("elhasMatchedConVetoLabel"));
    }
    if (m_useNeutrino){
        t_nuPt  = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("nuPtLabel"))
        t_nuEta = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("nuEtaLabel"))
        t_nuPhi = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("nuPhiLabel"))
    }
    if (m_useJets){
        m_jetPtMin     = iConfig.getParameter<double>("jetPtMin");
        m_jetAbsEtaMax = iConfig.getParameter<double>("jetAbsEtaMax");

        t_rho     = consumes<double>(iConfig.getParameter<edm::InputTag>("rhoLabel");
        t_jetPt   = consumes<vector<float>>(iConfig.getParameter<edm::InputTag>("jetPtLabel");
        t_jetEta  = consumes<vector<float>>(iConfig.getParameter<edm::InputTag>("jetEtaLabel");
        t_jetPhi  = consumes<vector<float>>(iConfig.getParameter<edm::InputTag>("jetPhiLabel");
        t_jetE    = consumes<vector<float>>(iConfig.getParameter<edm::InputTag>("jetEnergyLabel");
        t_jetCSV  = consumes<vector<float>>(iConfig.getParameter<edm::InputTag>("jetCSVLabel");
        t_jetCMVA = consumes<vector<float>>(iConfig.getParameter<edm::InputTag>("jetCMVALabel");
        t_jetCvsB = consumes<vector<float>>(iConfig.getParameter<edm::InputTag>("jetCvsBLabel");
        t_jetCvsL = consumes<vector<float>>(iConfig.getParameter<edm::InputTag>("jetCvsLLabel");
        t_jetJEC  = consumes<vector<float>>(iConfig.getParameter<edm::InputTag>("jetJECLabel");
        t_jetnHadEnergy     = consumes<vector<float>>(iConfig.getParameter<edm::InputTag>("jetnHadEnergyLabel");
        t_jetnEMEnergy      = consumes<vector<float>>(iConfig.getParameter<edm::InputTag>("jetnEMEnergyLabel");
        t_jetcHadEnergy     = consumes<vector<float>>(iConfig.getParameter<edm::InputTag>("jetcHadEnergyLabel");
        t_jetcEMEnergy      = consumes<vector<float>>(iConfig.getParameter<edm::InputTag>("jetcEMEnergyLabel");
        t_jetHadronFlavour  = consumes<vector<float>>(iConfig.getParameter<edm::InputTag>("jetPartonFlavourLabel");
        t_jetPartonFlavour  = consumes<vector<float>>(iConfig.getParameter<edm::InputTag>("jetHadronFlavourLabel");
        t_jetcMultip        = consumes<vector<float>>(iConfig.getParameter<edm::InputTag>("jetcMultipLabel");
        t_jetnMultip        = consumes<vector<float>>(iConfig.getParameter<edm::InputTag>("jetnMultipLabel");
        t_jetY              = consumes<vector<float>>(iConfig.getParameter<edm::InputTag>("jetYLabel");
        t_jetArea           = consumes<vector<float>>(iConfig.getParameter<edm::InputTag>("jetAreaLabel");
    }
    if (m_useLargeRJets){
    }
    if (m_isMC && m_useTruth){
        if (m_useJets){
            t_jetGenJetPt     = consumes<vector<float>>(iConfig.getParameter<edm::InputTag>("jetGenJetPtLabel"));
            t_jetGenJetEta    = consumes<vector<float>>(iConfig.getParameter<edm::InputTag>("jetGenJetEtaLabel"));
            t_jetGenJetPhi    = consumes<vector<float>>(iConfig.getParameter<edm::InputTag>("jetGenJetPhiLabel"));
            t_jetGenJetE      = consumes<vector<float>>(iConfig.getParameter<edm::InputTag>("jetGenJetELabel"));
            t_jetGenJetCharge = consumes<vector<float>>(iConfig.getParameter<edm::InputTag>("jetGenJetChargeLabel"));
        }
    }
    if (m_useTriggers){
    }
    // MET

    produces<cmaEvent>("cmaEvent");  // "cmaEvent" object that contains easy access to info
} // end constructor


CMAProducer::~CMAProducer() {}


void CMAProducer::beginJob(){
    /* Producer setup before the event loop */
    return;
}


void CMAProducer::produce(edm::Event& evt, const edm::EventSetup& ){
    /* Get the values from the event */
    edm::Handle<int> h_runno; 
    evt.getByToken(t_runno, h_runno);
    const int runno(*h_runno.product()) ; 


    // physics objects
    if (m_useLeptons){
        initialize_electrons(evt);
        initialize_muons(evt);
    }
    if (m_useNeutrino){
    }
    if (m_useJets){
    }
    if (m_useLargeRJets){
    }

    return;
}


void Event::endJob(){
    /* At the close */
    return;
}


void initialize_kinematics(edm::Event& evt){
    evt.getByToken (t_npv, h_npv);
}


void initialize_electrons(edm::Event& evt){
    /* Build electrons */
    m_electrons.clear();

    evt.getByToken(t_elDxy, h_elDxy);
    evt.getByToken(t_elDz,  h_elDz);
    evt.getByToken(t_elE,   h_elE);
    evt.getByToken(t_elEta, h_elEta);
    evt.getByToken(t_elHoE, h_elHoE);
    evt.getByToken(t_elKey, h_elKey);
    evt.getByToken(t_elPhi, h_elPhi);
    evt.getByToken(t_elPt,  h_elPt);
    evt.getByToken(t_eldEtaIn,      h_eldEtaIn);
    evt.getByToken(t_eldEtaInSeed,  h_eldEtaInSeed);
    evt.getByToken(t_elscEta,       h_elscEta);
    evt.getByToken(t_eldPhiIn,      h_eldPhiIn);
    evt.getByToken(t_elRelIsoEA,    h_elRelIsoEA);
    evt.getByToken(t_elCharge,      h_elCharge);
    evt.getByToken(t_elfull5x5siee, h_elfull5x5siee);
    evt.getByToken(t_elvidLoose,    h_elvidLoose);
    evt.getByToken(t_elvidMedium,   h_elvidMedium);
    evt.getByToken(t_elvidTight,    h_elvidTight);
    evt.getByToken(t_elvidVeto,     h_elvidVeto);
    evt.getByToken(t_elvidHEEP,     h_elvidHEEP);
    evt.getByToken(t_elmissHits,    h_elmissHits);
    evt.getByToken(t_elooEmooP,     h_elooEmooP);
    evt.getByToken(t_elhasMatchedConVeto, h_elhasMatchedConVeto);

    for (unsigned int iel=0, size=(h_elPt.product())->size(); iel<size; ++iel) {
        Electron el;

        double elPt        = (h_elPt.product())->at(iel) ; 
        double elscAbsEta  = std::abs((h_elscEta.product())->at(iel)) ; 
        double elRelIsoEA  = (h_elRelIsoEA.product())->at(iel) ; 
        double dEtaInSeed  = (h_eldEtaInSeed.product())->at(iel);
        double dPhiIn      = (h_eldPhiIn.product())->at(iel);
        double full5x5siee = (h_elfull5x5siee.product())->at(iel);
        double HoE = (h_elHoE.product())->at(iel);
        double Dxy = (h_elDxy.product())->at(iel);
        double Dz  = (h_elDz.product())->at(iel);
        double ooEmooP = (h_elooEmooP.product())->at(iel);
        double missHits = (h_elmissHits.product())->at(iel);
        bool hasMatchedConVeto = (h_elhasMatchedConVeto.product())->at(iel);
        bool isEB = (elscAbsEta <= 1.479);
        // electron ID

        el.p4.SetPtEtaPhiE();

        m_electrons.push_back(el);
    }

    return;
}


void initialize_jets(){
    /* Build jets */
    m_jets.clear();

    evt.getByToken (t_rho,       h_rho);
    evt.getByToken (t_jetPt,     h_jetPt);
    evt.getByToken (t_jetEta,    h_jetEta);
    evt.getByToken (t_jetPhi,    h_jetPhi);
    evt.getByToken (t_jetEnergy, h_jetEnergy);
    evt.getByToken (t_jetCSV,    h_jetCSV);
    evt.getByToken (t_jetCMVA,   h_jetCMVA);
    evt.getByToken (t_jetCvsB,   h_jetCvsB);
    evt.getByToken (t_jetCvsL,   h_jetCvsL);
    evt.getByToken (t_jetJEC,    h_jetJEC);
    evt.getByToken (t_jetY,      h_jetY);
    evt.getByToken (t_jetArea,   h_jetArea);
    evt.getByToken (t_jetnHadEnergy, h_jetnHadEnergy);
    evt.getByToken (t_jetnEMEnergy,  h_jetnEMEnergy);
    evt.getByToken (t_jetcHadEnergy, h_jetcHadEnergy);
    evt.getByToken (t_jetcEMEnergy,  h_jetcEMEnergy);
    evt.getByToken (t_jetcMultip,    h_jetcMultip);
    evt.getByToken (t_jetnMultip,    h_jetnMultip);
    evt.getByToken (t_jetHadronFlavour, h_jetHadronFlavour);
    evt.getByToken (t_jetPartonFlavour, h_jetPartonFlavour);

    for (unsigned ijet=0, size=(h_jetPt.product())->size(); ijet<size; ++ijet) { 
        Jet jet;

        // pT & |eta| min
        double jetPt  = (h_jetPt.product())->at(ijet);
        double jetEta = (h_jetEta.product())->at(ijet);
        if (abs(jetEta) > m_jetAbsEtaMax || jetPt < m_jetPtMin) continue; 

        jet.p4.SetPtEtaPhiE( jetPt, jetEta, (h_jetPhi.product())->at(ijet), (h_jetE.product())->at(ijet));

        m_jets.push_back(jet);
    }


    // Generator jets
    m_truth_jets.clear()
    if (m_isMC && m_useTruth){
    evt.getByToken (t_jetGenJetE,    h_jetGenJetE);
    evt.getByToken (t_jetGenJetEta,  h_jetGenJetEta);
    evt.getByToken (t_jetGenJetPt,   h_jetGenJetPt);
    evt.getByToken (t_jetGenJetPhi,  h_jetGenJetPhi);
    }

    return;
}

void initialize_largeRjets(){
    // Generator jets
    m_truth_largeRJets.clear()
    if (m_isMC && m_useTruth){
    }
    return;
}

void initialize_neutrino(){
    return;
}


// THE END
