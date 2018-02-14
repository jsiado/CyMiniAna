/*
Created:        --
Last Updated:   18 December 2017

Dan Marley
daniel.edison.marley@cernSPAMNOT.ch
Texas A&M University

-----

Build Large-R Jets from EDMntuples
*/
#include "Analysis/CyMiniAna/interface/LargeRJets.h"


using namespace edm; 


LargeRJets::LargeRJets(edm::ParameterSet const& iConfig, edm::ConsumesCollector && iC) : 
  m_labels(iConfig.getParameter<edm::ParameterSet>("largeRjetLabels")),
  m_isMC(iConfig.getParameter<bool>("isMC")),
  m_useTruth(iConfig.getParameter<bool>("useTruth")),
  m_data_path(iConfig.getParameter<std::string>("data_path")){
    t_ljetPt  = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("ljetPtLabel"));
    t_ljetEta = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("ljetEtaLabel"));
    t_ljetPhi = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("ljetPhiLabel"));
    t_ljetE   = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("ljetEnergyLabel"));
    t_ljetCSV  = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("ljetCSVLabel"));
    t_ljetCMVA = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("ljetCMVALabel"));
    t_ljetCvsB = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("ljetCvsBLabel"));
    t_ljetCvsL = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("ljetCvsLLabel"));
    t_ljetJEC  = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("ljetJECLabel"));
    t_ljetJECsyst = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("ljetJECsystLabel"));
    t_ljetJERSF   = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("ljetJERSFLabel"));
    t_ljetJERSFUp = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("ljetJERSFUpLabel"));
    t_ljetJERSFDown = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("ljetJERSFDownLabel"));
    t_ljetY = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("ljetYLabel"));
    t_ljetArea = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("ljetAreaLabel"));
    t_ljetMuonEnergy = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("ljetMuonEnergyLabel"));
    t_ljetnHadEnergy = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("ljetnHadEnergyLabel"));
    t_ljetnEMEnergy = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("ljetnEMEnergyLabel"));
    t_ljetcHadEnergy = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("ljetcHadEnergyLabel"));
    t_ljetcEMEnergy = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("ljetcEMEnergyLabel"));
    t_ljetcMultip = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("ljetcMultipLabel"));
    t_ljetnMultip = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("ljetnMultipLabel"));
    t_ljetPartonFlavour = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("ljetHadronFlavourLabel"));
    t_ljetHadronFlavour = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("ljetPartonFlavourLabel"));
    t_ljettau1 = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("ljettau1Label"));
    t_ljettau2 = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("ljettau2Label"));
    t_ljettau3 = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("ljettau3Label"));
    t_ljetPrunedMass = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("ljetPrunedMassLabel"));
    t_ljetTrimmedMass = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("ljetTrimmedMassLabel"));
    t_ljetFilteredMass = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("ljetFilteredMassLabel"));
    t_ljetSoftDropMass = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("ljetSoftDropMassLabel"));
    t_ljet_subjetIdx0 = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("ljet_subjetIdx0Label"));
    t_ljet_subjetIdx1 = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("ljet_subjetIdx1Label"));
    t_ljet_subjetHadFl = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("ljet_subjetHadronFlavourLabel"));
    t_ljet_subjetPt = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("ljet_subjetPtLabel"));
    t_ljet_subjetEta = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("ljet_subjetEtaLabel"));
    t_ljet_subjetPhi = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("ljet_subjetPhiLabel"));
    t_ljet_subjetE = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("ljet_subjetEnergyLabel"));
    t_ljet_subjetJEC = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("ljet_subjetJECLabel"));
    t_ljet_subjetCSV = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("ljet_subjetCSVLabel"));
    t_ljet_subjetCMVA = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("ljet_subjetCMVALabel"));
    if (m_isMC && m_useTruth){
        t_ljetGenPt  = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("ljetGenPtLabel"));
        t_ljetGenEta = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("ljetGenEtaLabel"));
        t_ljetGenPhi = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("ljetGenPhiLabel"));
        t_ljetGenE   = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("ljetGenELabel"));
        t_ljetGenCharge = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("ljetGenChargeLabel"));
        t_ljet_subjetGenPt  = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("ljet_subjetGenPtLabel"));
        t_ljet_subjetGenEta = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("ljet_subjetGenEtaLabel"));
        t_ljet_subjetGenPhi = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("ljet_subjetGenPhiLabel"));
        t_ljet_subjetGenE   = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("ljet_subjetGenELabel"));
        t_ljet_subjetGenCharge = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("ljet_subjetGenChargeLabel"));
    }

    m_btagTool = new BTagTools(true,m_data_path);
}

LargeRJets::~LargeRJets() {}


std::vector<Ljet> LargeRJets::execute(const edm::Event& evt, const objectSelection& obj){
    /* Build the large-R jets */
    m_ljets.clear();

    evt.getByToken(t_ljetPt,     h_ljetPt);
    evt.getByToken(t_ljetEta,    h_ljetEta);
    evt.getByToken(t_ljetPhi,    h_ljetPhi);
    evt.getByToken(t_ljetE,      h_ljetE);
    evt.getByToken(t_ljetCSV,    h_ljetCSV);
    evt.getByToken(t_ljetCMVA,   h_ljetCMVA);
    evt.getByToken(t_ljetCvsB,   h_ljetCvsB);
    evt.getByToken(t_ljetCvsL,   h_ljetCvsL);
    evt.getByToken(t_ljetJEC,    h_ljetJEC);
    evt.getByToken(t_ljetJECsyst,h_ljetJECsyst);
    evt.getByToken(t_ljetJERSF,  h_ljetJERSF);
    evt.getByToken(t_ljetJERSFUp,h_ljetJERSFUp);
    evt.getByToken(t_ljetJERSFDown, h_ljetJERSFDown);
    evt.getByToken(t_ljetY,      h_ljetY);
    evt.getByToken(t_ljetArea,   h_ljetArea);
    evt.getByToken(t_ljetnHadEnergy, h_ljetnHadEnergy);
    evt.getByToken(t_ljetnEMEnergy,  h_ljetnEMEnergy);
    evt.getByToken(t_ljetcHadEnergy, h_ljetcHadEnergy);
    evt.getByToken(t_ljetcEMEnergy,  h_ljetcEMEnergy);
    evt.getByToken(t_ljetcMultip,    h_ljetcMultip);
    evt.getByToken(t_ljetnMultip,    h_ljetnMultip);
    evt.getByToken(t_ljetMuonEnergy, h_ljetMuonEnergy);
    evt.getByToken(t_ljetHadronFlavour, h_ljetHadronFlavour);
    evt.getByToken(t_ljetPartonFlavour, h_ljetPartonFlavour);
    evt.getByToken(t_ljettau1, h_ljettau1);
    evt.getByToken(t_ljettau2, h_ljettau2);
    evt.getByToken(t_ljettau3, h_ljettau3);
    evt.getByToken(t_ljetSoftDropMass, h_ljetSoftDropMass);
    evt.getByToken(t_ljet_subjetIdx0,  h_ljet_subjetIdx0);
    evt.getByToken(t_ljet_subjetIdx1,  h_ljet_subjetIdx1);
    evt.getByToken(t_ljet_subjetHadFl, h_ljet_subjetHadFl);
    evt.getByToken(t_ljet_subjetJEC,   h_ljet_subjetJEC);
    evt.getByToken(t_ljet_subjetCSV,   h_ljet_subjetCSV);
    evt.getByToken(t_ljet_subjetCMVA,  h_ljet_subjetCMVA);
    evt.getByToken(t_ljet_subjetPt,    h_ljet_subjetPt);
    evt.getByToken(t_ljet_subjetEta,   h_ljet_subjetEta);
    evt.getByToken(t_ljet_subjetPhi,   h_ljet_subjetPhi);
    evt.getByToken(t_ljet_subjetE,     h_ljet_subjetE);


    for (unsigned ijet=0, size=(h_ljetPt.product())->size(); ijet<size; ++ijet) {
        Ljet ljet;

        ljet.p4.SetPtEtaPhiE( (h_ljetPt.product())->at(ijet), (h_ljetEta.product())->at(ijet), 
                              (h_ljetPhi.product())->at(ijet), (h_ljetE.product())->at(ijet));

        // Substructure
        ljet.tau1 = (h_ljettau1.product())->at(ijet);
        ljet.tau2 = (h_ljettau2.product())->at(ijet);
        ljet.tau3 = (h_ljettau3.product())->at(ijet);
        ljet.tau21 = ljet.tau2 / ljet.tau1;
        ljet.tau32 = ljet.tau3 / ljet.tau2;
        ljet.softDropMass = (h_ljetSoftDropMass.product())->at(ijet);
        ljet.CSVv2 = (h_ljetCSV.product())->at(ijet);

        // Soft-drop subjets
        ljet.subjets.clear();
        std::vector<int> indices = { int((h_ljet_subjetIdx0.product())->at(ijet)),
                                     int((h_ljet_subjetIdx1.product())->at(ijet)) };

        for (const auto& idx : indices){
            if (idx < 0) continue;
            Jet subjet;
            subjet.p4.SetPtEtaPhiE( (h_ljet_subjetPt.product())->at(idx), (h_ljet_subjetEta.product())->at(idx), 
                                    (h_ljet_subjetPhi.product())->at(idx), (h_ljet_subjetE.product())->at(idx));

            subjet.cMVAv2 = (h_ljet_subjetCMVA.product())->at(idx);
            subjet.CSVv2  = (h_ljet_subjetCSV.product())->at(idx);

            subjet.isbtagged = { {"L",false}, {"M",false}, {"T",false} };
            m_btagTool->getBTagDecisions(subjet);
            std::map<std::string,double> SFs = m_btagTool->execute(subjet);

            subjet.btagSF    = SFs.at("central");
            subjet.btagSF_UP = SFs.at("up");
            subjet.btagSF_DN = SFs.at("down");

            ljet.subjets.push_back(subjet);
        }

        // ID
        ljet.cMultip = (h_ljetcMultip.product())->at(ijet);
        ljet.nMultip = (h_ljetnMultip.product())->at(ijet);
        ljet.nEMEnergy = (h_ljetnEMEnergy.product())->at(ijet);
        ljet.cEMEnergy = (h_ljetcEMEnergy.product())->at(ijet);
        ljet.nHadEnergy = (h_ljetnHadEnergy.product())->at(ijet);
        ljet.cHadEnergy = (h_ljetcHadEnergy.product())->at(ijet);
        ljet.muonEnergy = (h_ljetMuonEnergy.product())->at(ijet);

        // JEC & JER
        ljet.JECsyst  = (h_ljetJECsyst.product())->at(ijet);
        ljet.JERSF    = (h_ljetJERSF.product())->at(ijet);
        ljet.JERSF_UP = (h_ljetJERSFUp.product())->at(ijet);
        ljet.JERSF_DN = (h_ljetJERSFDown.product())->at(ijet);

        setLjetID(ljet);
       	bool passObjSel	= obj.pass(ljet);
        if (!passObjSel) continue;

        m_ljets.push_back(ljet);
    }

    return m_ljets;
}


std::vector<Ljet> LargeRJets::execute_truth(const edm::Event& evt, const objectSelection& obj){
    /* Build Generator large-R jets */
    m_truth_ljets.clear();

    if (!m_isMC || !m_useTruth) return m_truth_ljets;

    evt.getByToken(t_ljetGenPt,  h_ljetGenPt);
    evt.getByToken(t_ljetGenEta, h_ljetGenEta);
    evt.getByToken(t_ljetGenPhi, h_ljetGenPhi);
    evt.getByToken(t_ljetGenE,   h_ljetGenE);
    evt.getByToken(t_ljetGenCharge, h_ljetGenCharge);

    for (unsigned ijet=0, size=(h_ljetGenPt.product())->size(); ijet<size; ++ijet) {
        Ljet ljet;

        if ( std::abs((h_ljetGenEta.product())->at(ijet)) > 10) continue;  // protect against ROOT errors

        ljet.p4.SetPtEtaPhiE( (h_ljetGenPt.product())->at(ijet),  (h_ljetGenEta.product())->at(ijet),
                              (h_ljetGenPhi.product())->at(ijet), (h_ljetGenE.product())->at(ijet));
        ljet.charge = (h_ljetGenCharge.product())->at(ijet);

       	bool passObjSel	= obj.pass(ljet,true);
        if (!passObjSel) continue;

        m_truth_ljets.push_back(ljet);
    }

    return m_truth_ljets;
}


void LargeRJets::setLjetID(Ljet& ljet) const{
    /* Large-R Jet ID
       Use the same recommendations as AK4 jets
         https://twiki.cern.ch/twiki/bin/view/CMS/JetID13TeVRun2016
     */
    float absEta = std::abs(ljet.p4.Eta());
    float NHF  = ljet.nHadEnergy;
    float CHF  = ljet.cHadEnergy;
    float MUF  = ljet.muonEnergy / ljet.p4.E();
    float CHM  = ljet.cMultip;
    float CEMF = ljet.cEMEnergy;
    float NEMF = ljet.nEMEnergy;
    int NumConst = ljet.cMultip+ljet.nMultip;

    // only care about jets |eta|<2.4
    bool looseJetID = (NHF<0.99 && NEMF<0.99 && NumConst>1) && ((absEta<=2.4 && CHF>0 && CHM>0 && CEMF<0.99) || absEta>2.4) && absEta<=2.7;
    bool tightJetID = (NHF<0.90 && NEMF<0.90 && NumConst>1) && ((absEta<=2.4 && CHF>0 && CHM>0 && CEMF<0.99) || absEta>2.4) && absEta<=2.7;
    bool tightLepVetoJetID = (NHF<0.90 && NEMF<0.90 && NumConst>1 && MUF<0.8) && ((absEta<=2.4 && CHF>0 && CHM>0 && CEMF<0.90) || absEta>2.4) && absEta<=2.7;

    ljet.loose = looseJetID;
    ljet.tight = tightJetID;
    ljet.tightlepveto = tightLepVetoJetID;

    return;
}

// THE END
