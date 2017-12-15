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
    t_rho   = consumes<double>(iConfig.getParameter<edm::InputTag>("rhoLabel");

    if (m_useLeptons){
        m_elPtMin     = iConfig.getParameter<double>("elPtMin");
        m_elAbsEtaMax = iConfig.getParameter<double>("elAbsEtaMax");
        m_applyIso    = iConfig.getParameter<bool>("applyIso");

        m_muPtMin     = iConfig.getParameter<double>("muPtMin");
        m_muAbsEtaMax = iConfig.getParameter<double>("muAbsEtaMax");
        m_muIsoMin    = iConfig.getParameter<double>("muIsoMin");
        m_muIsoMax    = iConfig.getParameter<double>("muIsoMax");

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

        t_muDxy = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("muDxyLabel"))), 
        t_muDz  = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("muDzLabel"))), 
        t_muE   = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("muELabel"))), 
        t_muEta = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("muEtaLabel"))), 
        t_muPhi = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("muPhiLabel"))), 
        t_muPt  = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("muPtLabel"))), 
        t_muKey = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("muKeyLabel"))), 
        t_muIso04   = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("muIso04Label"))), 
        t_muCharge  = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("muChargeLabel"))), 
        t_muSumPUPt = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("muSumPUPtLabel"))), 
        t_muSumPhotonPt    = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("muSumPhotonPtLabel"))), 
        t_muGlbTrkNormChi2 = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("muGlbTrkNormChi2Label"))), 
        t_muInTrkNormChi2  = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("muInTrkNormChi2Label"))), 
        t_muIsGlobalMuon   = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("muIsGlobalMuonLabel"))), 
        t_muIsLooseMuon    = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("muIsLooseMuonLabel"))), 
        t_muIsPFMuon       = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("muIsPFMuonLabel"))), 
        t_muIsSoftMuon     = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("muIsSoftMuonLabel"))), 
        t_muIsTightMuon    = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("muIsTightMuonLabel"))), 
        t_muIsTrackerMuon  = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("muIsTrackerMuonLabel"))), 
        t_muNumberOfPixelLayers  = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("muNumberOfPixelLayersLabel"))), 
        t_muNumberTrackerLayers  = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("muNumberTrackerLayersLabel"))), 
        t_muNumberValidMuonHits  = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("muNumberValidMuonHitsLabel"))), 
        t_muNumberValidPixelHits = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("muNumberValidPixelHitsLabel"))), 
        t_muSumChargedHadronPt   = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("muSumChargedHadronPtLabel"))), 
        t_muSumNeutralHadronPt   = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("muSumNeutralHadronPtLabel"))), 
        t_muNumberMatchedStations    = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("muNumberMatchedStationsLabel"))), 
        t_muNumberOfValidTrackerHits = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("muNumberOfValidTrackerHitsLabel"))), 

        if (m_useTruth){
            t_muGenMuonE      = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("muGenMuonELabel"))), 
            t_muGenMuonEta    = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("muGenMuonEtaLabel"))), 
            t_muGenMuonPhi    = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("muGenMuonPhiLabel"))), 
            t_muGenMuonPt     = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("muGenMuonPtLabel"))), 
            t_muGenMuonCharge = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("muGenMuonChargeLabel"))), 
        }
    }
    if (m_useNeutrino && !m_kinematicReco){
        // neutrinos reconstructed from MET
        t_nuPt  = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("nuPtLabel"))
        t_nuEta = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("nuEtaLabel"))
        t_nuPhi = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("nuPhiLabel"))
    }
    if (m_useJets){
        // AK4 Jets
        m_jetPtMin     = iConfig.getParameter<double>("jetPtMin");
        m_jetAbsEtaMax = iConfig.getParameter<double>("jetAbsEtaMax");

        t_jetPt   = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetPtLabel");
        t_jetEta  = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetEtaLabel");
        t_jetPhi  = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetPhiLabel");
        t_jetE    = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetEnergyLabel");
        t_jetCSV  = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetCSVLabel");
        t_jetCMVA = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetCMVALabel");
        t_jetCvsB = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetCvsBLabel");
        t_jetCvsL = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetCvsLLabel");
        t_jetJEC  = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetJECLabel");
        t_jetY    = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetYLabel");
        t_jetArea = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetAreaLabel");
        t_jetnHadEnergy = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetnHadEnergyLabel");
        t_jetnEMEnergy  = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetnEMEnergyLabel");
        t_jetcHadEnergy = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetcHadEnergyLabel");
        t_jetcEMEnergy  = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetcEMEnergyLabel");
        t_jetcMultip    = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetcMultipLabel");
        t_jetnMultip    = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetnMultipLabel");
        t_jetHadronFlavour  = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetPartonFlavourLabel");
        t_jetPartonFlavour  = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetHadronFlavourLabel");

        if (m_useTruth){
            t_jetGenJetPt     = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetGenJetPtLabel"));
            t_jetGenJetEta    = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetGenJetEtaLabel"));
            t_jetGenJetPhi    = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetGenJetPhiLabel"));
            t_jetGenJetE      = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetGenJetELabel"));
            t_jetGenJetCharge = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetGenJetChargeLabel"));
        }
    }
    if (m_useLargeRJets){
        // AK8 Jets
        m_ljetPtMin     = iConfig.getParameter<double>("ljetPtMin");
        m_ljetAbsEtaMax = iConfig.getParameter<double>("ljetAbsEtaMax");

        t_ljetPt   = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("ljetPtLabel");
        t_ljetEta  = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("ljetEtaLabel");
        t_ljetPhi  = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("ljetPhiLabel");
        t_ljetE    = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("ljetEnergyLabel");
        t_ljetCSV  = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("ljetCSVLabel");
        t_ljetCMVA = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("ljetCMVALabel");
        t_ljetCvsB = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("ljetCvsBLabel");
        t_ljetCvsL = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("ljetCvsLLabel");
        t_ljetJEC  = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("ljetJECLabel");
        t_ljetY    = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("ljetYLabel");
        t_ljetArea = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("ljetAreaLabel");
        t_ljetnHadEnergy = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("ljetnHadEnergyLabel");
        t_ljetnEMEnergy  = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("ljetnEMEnergyLabel");
        t_ljetcHadEnergy = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("ljetcHadEnergyLabel");
        t_ljetcEMEnergy  = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("ljetcEMEnergyLabel");
        t_ljetcMultip    = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("ljetcMultipLabel");
        t_ljetnMultip    = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("ljetnMultipLabel");
        t_ljetHadronFlavour = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("ljetPartonFlavourLabel");
        t_ljetPartonFlavour = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("ljetHadronFlavourLabel");
        t_ljettau1          = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag> ("ljettau1Label");
        t_ljettau2          = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag> ("ljettau2Label");
        t_ljettau3          = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag> ("ljettau3Label");
        t_ljetPrunedMass    = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag> ("ljetPrunedMassLabel");
        t_ljetTrimmedMass   = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag> ("ljetTrimmedMassLabel");
        t_ljetFilteredMass  = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag> ("ljetFilteredMassLabel");
        t_ljetSoftDropMass  = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag> ("ljetSoftDropMassLabel");
        t_ljet_subjetIdx0   = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag> ("ljet_subjetIdx0Label");
        t_ljet_subjetIdx1   = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag> ("ljet_subjetIdx1Label");
        t_ljet_subjetHadFl  = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag> ("ljet_subjetHadronFlavourLabel");
        t_ljet_subjetPt     = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag> ("ljet_subjetPtLabel");
        t_ljet_subjetEta    = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag> ("ljet_subjetEtaLabel");
        t_ljet_subjetPhi    = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag> ("ljet_subjetPhiLabel");
        t_ljet_subjetEnergy = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag> ("ljet_subjetEnergyLabel");
        t_ljet_subjetJEC    = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag> ("ljet_subjetJECLabel");
        t_ljet_subjetCSV    = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag> ("ljet_subjetCSVLabel");
        t_ljet_subjetCMVA   = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag> ("ljet_subjetCMVALabel");

        if (m_useTruth){
            t_ljetGenJetPt     = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("ljetGenJetPtLabel"));
            t_ljetGenJetEta    = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("ljetGenJetEtaLabel"));
            t_ljetGenJetPhi    = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("ljetGenJetPhiLabel"));
            t_ljetGenJetE      = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("ljetGenJetELabel"));
            t_ljetGenJetCharge = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("ljetGenJetChargeLabel"));
            t_ljet_subjetGenJetPt     = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag> ("ljet_subjetGenJetPtLabel");
            t_ljet_subjetGenJetEta    = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag> ("ljet_subjetGenJetEtaLabel");
            t_ljet_subjetGenJetPhi    = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag> ("ljet_subjetGenJetPhiLabel");
            t_ljet_subjetGenJetE      = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag> ("ljet_subjetGenJetELabel");
            t_ljet_subjetGenJetCharge = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag> ("ljet_subjetGenJetChargeLabel");
        }
    }
    if (m_useTriggers){
    }
    // MET


    if (m_isMC){
    }

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


void initialize_muons(){
    /* Build muons */
    m_muons.clear();

    evt.getByToken(t_muDxy, h_muDxy);
    evt.getByToken(t_muDz,  h_muDz);
    evt.getByToken(t_muE,   h_muE);
    evt.getByToken(t_muEta, h_muEta);
    evt.getByToken(t_muPhi, h_muPhi);
    evt.getByToken(t_muPt,  h_muPt);
    evt.getByToken(t_muKey, h_muKey);
    evt.getByToken(t_muIso04,   h_muIso04);
    evt.getByToken(t_muSumPUPt, h_muSumPUPt);
    evt.getByToken(t_muCharge,  h_muCharge);
    evt.getByToken(t_muSumPhotonPt,    h_muSumPhotonPt);
    evt.getByToken(t_muGlbTrkNormChi2, h_muGlbTrkNormChi2);
    evt.getByToken(t_muInTrkNormChi2,  h_muInTrkNormChi2);
    evt.getByToken(t_muIsGlobalMuon,   h_muIsGlobalMuon);
    evt.getByToken(t_muIsLooseMuon,    h_muIsLooseMuon);
    evt.getByToken(t_muIsPFMuon,       h_muIsPFMuon);
    evt.getByToken(t_muIsSoftMuon,     h_muIsSoftMuon);
    evt.getByToken(t_muIsTightMuon,    h_muIsTightMuon);
    evt.getByToken(t_muIsTrackerMuon,  h_muIsTrackerMuon);
    evt.getByToken(t_muNumberOfPixelLayers,  h_muNumberOfPixelLayers);
    evt.getByToken(t_muNumberTrackerLayers,  h_muNumberTrackerLayers);
    evt.getByToken(t_muNumberValidMuonHits,  h_muNumberValidMuonHits);
    evt.getByToken(t_muNumberValidPixelHits, h_muNumberValidPixelHits);
    evt.getByToken(t_muSumChargedHadronPt,   h_muSumChargedHadronPt);
    evt.getByToken(t_muSumNeutralHadronPt,   h_muSumNeutralHadronPt);
    evt.getByToken(t_muNumberMatchedStations,    h_muNumberMatchedStations);
    evt.getByToken(t_muNumberOfValidTrackerHits, h_muNumberOfValidTrackerHits);

    for (unsigned int imu=0, size=(h_muPt.product())->size(); imu<size; ++imu) {
        double muPt  = (h_muPt.product())->at(imu);
        double muEta = (h_muEta.product())->at(imu);
        double muIso = (h_muIso04.product())->at(imu);

        bool passMuId(false);
        bool isLooseMu((h_muIsLooseMuon.product())->at(imu) > 0);
        bool isTightMu((h_muIsTightMuon.product())->at(imu) > 0);

        if (type_ == LOOSE && isLooseMu)      passMuId = true;
        else if (type_ == TIGHT && isTightMu) passMuId = true;
        else passMuId = false;

        bool passMuISO(false);
        if ( muIso >= m_muIsoMin && muIso < m_muIsoMax ) passMuISO = true;

        // kinematics, ID, and isolation
        if ( muPt < m_muPtMin || abs(muEta) > m_muAbsEtaMax || !passMuID || !passMuISO )
             continue;

        Muon mu;
        mu.p4.SetPtEtaPhiE( muPt, muEta, (h_muPhi.product())->at(imu), (h_muE.product())->at(imu) );
        /* ... */

        m_muons.push_back(muon) ; 
    }


    m_truth_muons.clear();
    if (m_useTruth){
        evt.getByToken(t_muGenMuonE,   h_muGenMuonE);
        evt.getByToken(t_muGenMuonEta, h_muGenMuonEta);
        evt.getByToken(t_muGenMuonPhi, h_muGenMuonPhi);
        evt.getByToken(t_muGenMuonPt,  h_muGenMuonPt);
        evt.getByToken(t_muGenMuonCharge, h_muGenMuonCharge);

        for (unsigned int imu=0, size=(h_muGenMuonPt.product())->size(); imu<size; ++imu) {
            Muon tru_mu;
            tru_mu.p4.SetPtEtaPhiE( (h_muGenMuonPt.product())->at(imu),  (h_muGenMuonEta.product())->at(imu), 
                                    (h_muGenMuonPhi.product())->at(imu), (h_muGenMuonE.product())->at(imu) );
            tru_mu.charge = (h_muGenMuonCharge.product())->at(imu);
            m_truth_muons.push_back(tru_mu);
        }
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
