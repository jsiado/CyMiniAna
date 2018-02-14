/*
Created:        --
Last Updated:   18 December 2017

Dan Marley
daniel.edison.marley@cernSPAMNOT.ch
Texas A&M University

-----

Build Jets from EDMntuples
*/
#include "Analysis/CyMiniAna/interface/Jets.h"


using namespace edm; 


Jets::Jets(edm::ParameterSet const& iConfig, edm::ConsumesCollector && iC) : 
  m_labels(iConfig.getParameter<edm::ParameterSet>("jetLabels")),
  m_isMC(iConfig.getParameter<bool>("isMC")),
  m_useTruth(iConfig.getParameter<bool>("useTruth")),
  m_data_path(iConfig.getParameter<std::string>("data_path")){
    t_jetPt  = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("jetPtLabel"));
    t_jetEta = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("jetEtaLabel"));
    t_jetPhi = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("jetPhiLabel"));
    t_jetE   = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("jetEnergyLabel"));
    t_jetHadronFlavour = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("jetPartonFlavourLabel"));
    t_jetPartonFlavour = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("jetHadronFlavourLabel"));
    t_jetCSV  = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("jetCSVLabel"));
    t_jetCMVA = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("jetCMVALabel"));
    t_jetCvsB = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("jetCvsBLabel"));
    t_jetCvsL = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("jetCvsLLabel"));
    t_jetJEC  = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("jetJECLabel"));
    t_jetJECsyst = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("jetJECsystLabel"));
    t_jetJERSF   = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("jetJERSFLabel"));
    t_jetJERSFUp = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("jetJERSFUpLabel"));
    t_jetJERSFDown  = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("jetJERSFDownLabel"));
    t_jetnHadEnergy = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("jetnHadEnergyLabel"));
    t_jetnEMEnergy  = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("jetnEMEnergyLabel"));
    t_jetcHadEnergy = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("jetcHadEnergyLabel"));
    t_jetcEMEnergy = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("jetcEMEnergyLabel"));
    t_jetcMultip = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("jetcMultipLabel"));
    t_jetnMultip = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("jetnMultipLabel"));
    t_jetY = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("jetYLabel"));
    t_jetArea = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("jetAreaLabel"));
    t_jetMuonEnergy = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag> ("jetMuonEnergyLabel"));
    if (m_isMC && m_useTruth){
      t_jetGenPt  = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("jetGenPtLabel"));
      t_jetGenEta = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("jetGenEtaLabel"));
      t_jetGenPhi = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("jetGenPhiLabel"));
      t_jetGenE   = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("jetGenELabel"));
      t_jetGenCharge = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("jetGenChargeLabel"));
    }

    m_btagTool = new BTagTools(false,m_data_path);
}

Jets::~Jets() {}


std::vector<Jet> Jets::execute(const edm::Event& evt, const objectSelection& obj){
    /* Build the jets */
    m_jets.clear();

    evt.getByToken(t_jetY,   h_jetY);
    evt.getByToken(t_jetPt,  h_jetPt);
    evt.getByToken(t_jetEta, h_jetEta);
    evt.getByToken(t_jetPhi, h_jetPhi);
    evt.getByToken(t_jetJEC, h_jetJEC);
    evt.getByToken(t_jetJECsyst, h_jetJECsyst);
    evt.getByToken(t_jetJERSF, h_jetJERSF);
    evt.getByToken(t_jetJERSFUp, h_jetJERSFUp);
    evt.getByToken(t_jetJERSFDown, h_jetJERSFDown);
    evt.getByToken(t_jetCSV, h_jetCSV);
    evt.getByToken(t_jetCMVA,   h_jetCMVA);
    evt.getByToken(t_jetCvsB,   h_jetCvsB);
    evt.getByToken(t_jetCvsL,   h_jetCvsL);
    evt.getByToken(t_jetArea,   h_jetArea);
    evt.getByToken(t_jetE,      h_jetE);
    evt.getByToken(t_jetMuonEnergy, h_jetMuonEnergy);
    evt.getByToken(t_jetnHadEnergy, h_jetnHadEnergy);
    evt.getByToken(t_jetnEMEnergy,  h_jetnEMEnergy);
    evt.getByToken(t_jetcHadEnergy, h_jetcHadEnergy);
    evt.getByToken(t_jetcEMEnergy,  h_jetcEMEnergy);
    evt.getByToken(t_jetcMultip,    h_jetcMultip);
    evt.getByToken(t_jetnMultip,    h_jetnMultip);
    evt.getByToken(t_jetHadronFlavour, h_jetHadronFlavour);
    evt.getByToken(t_jetPartonFlavour, h_jetPartonFlavour);

    for (unsigned ijet=0, size=(h_jetPt.product())->size(); ijet<size; ++ijet) {
        Jet jet;

        jet.p4.SetPtEtaPhiE((h_jetPt.product())->at(ijet),  (h_jetEta.product())->at(ijet), 
                            (h_jetPhi.product())->at(ijet), (h_jetE.product())->at(ijet));

        jet.cMVAv2      = (h_jetCMVA.product())->at(ijet);
        jet.CSVv2       = (h_jetCSV.product())->at(ijet);

        jet.cMultip     = (h_jetcMultip.product())->at(ijet);
        jet.nMultip     = (h_jetnMultip.product())->at(ijet);
        jet.nEMEnergy   = (h_jetnEMEnergy.product())->at(ijet);
        jet.nHadEnergy  = (h_jetnHadEnergy.product())->at(ijet);
        jet.cEMEnergy   = (h_jetcEMEnergy.product())->at(ijet);
        jet.cHadEnergy  = (h_jetcHadEnergy.product())->at(ijet);
        jet.muonEnergy  = (h_jetcMultip.product())->at(ijet);
        jet.true_flavor = int((h_jetHadronFlavour.product())->at(ijet));
        jet.index       = ijet;

        // JEC & JER
        jet.JECsyst = (h_jetJECsyst.product())->at(ijet);
        jet.JERSF    = (h_jetJERSF.product())->at(ijet);
        jet.JERSF_UP = (h_jetJERSFUp.product())->at(ijet);
        jet.JERSF_DN = (h_jetJERSFDown.product())->at(ijet);

        setJetID(jet);
        bool passObjSel = obj.pass(jet);

        if (!passObjSel) continue;

        // b-tagging SF
        jet.isbtagged = { {"L",false}, {"M",false}, {"T",false} };
        m_btagTool->getBTagDecisions(jet);
        std::map<std::string,double> SFs = m_btagTool->execute(jet);

        jet.btagSF    = SFs.at("central");
        jet.btagSF_UP = SFs.at("up");
        jet.btagSF_DN = SFs.at("down");

        m_jets.push_back(jet);
    }

    return m_jets;
}


std::vector<Jet> Jets::execute_truth(const edm::Event& evt, const objectSelection& obj){
    /* Build Generator jets */
    m_truth_jets.clear();

    if (!m_isMC || !m_useTruth) return m_truth_jets;  // return empty vector for non-truth setups

    evt.getByToken(t_jetGenPt,  h_jetGenPt);
    evt.getByToken(t_jetGenEta, h_jetGenEta);
    evt.getByToken(t_jetGenPhi, h_jetGenPhi);
    evt.getByToken(t_jetGenE,   h_jetGenE);
    evt.getByToken(t_jetGenCharge, h_jetGenCharge);

    for (unsigned ijet=0, size=(h_jetGenPt.product())->size(); ijet<size; ++ijet) {
        Jet jet;
        if ( std::abs((h_jetGenEta.product())->at(ijet)) > 10 ) continue;  // protect against ROOT errors

        jet.p4.SetPtEtaPhiE( (h_jetGenPt.product())->at(ijet),  (h_jetGenEta.product())->at(ijet),
                             (h_jetGenPhi.product())->at(ijet), (h_jetGenE.product())->at(ijet));
        jet.charge = (h_jetGenCharge.product())->at(ijet);

       	bool passObjSel	= obj.pass(jet,true);
        if (!passObjSel) continue;

        m_truth_jets.push_back(jet);
    }

    return m_truth_jets;
}


void Jets::setJetID(Jet& jet) const{
    /* Jet ID
       https://twiki.cern.ch/twiki/bin/view/CMS/JetID13TeVRun2016
     */
    float absEta = std::abs(jet.p4.Eta());
    float NHF  = jet.nHadEnergy;
    float CHF  = jet.cHadEnergy;
    float MUF  = jet.muonEnergy / jet.p4.E();
    float CHM  = jet.cMultip;
    float CEMF = jet.cEMEnergy;
    float NEMF = jet.nEMEnergy;
    int NumConst = jet.cMultip+jet.nMultip;

    // only care about jets |eta|<2.4
    bool looseJetID = (NHF<0.99 && NEMF<0.99 && NumConst>1) && ((absEta<=2.4 && CHF>0 && CHM>0 && CEMF<0.99) || absEta>2.4) && absEta<=2.7;
    bool tightJetID = (NHF<0.90 && NEMF<0.90 && NumConst>1) && ((absEta<=2.4 && CHF>0 && CHM>0 && CEMF<0.99) || absEta>2.4) && absEta<=2.7;
    bool tightLepVetoJetID = (NHF<0.90 && NEMF<0.90 && NumConst>1 && MUF<0.8) && ((absEta<=2.4 && CHF>0 && CHM>0 && CEMF<0.90) || absEta>2.4) && absEta<=2.7;

    jet.loose = looseJetID;
    jet.tight = tightJetID;
    jet.tightlepveto = tightLepVetoJetID;

    return;
}

// THE END
