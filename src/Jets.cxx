/*
Created:        --
Last Updated:   18 December 2017

Dan Marley
daniel.edison.marley@cernSPAMNOT.ch
Texas A&M University

-----

Build Electrons from EDMntuples
*/
#include "Analysis/CyMiniAna/interface/Jets.h"


using namespace edm; 


Jets::Jets(edm::ParameterSet const& iConfig, edm::ConsumesCollector && iC) : 
  m_useTruth(iConfig.getParameter<bool>("useTruth")),
  t_jetPt(iC.consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetPtLabel"))),
  t_jetEta(iC.consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetEtaLabel"))),
  t_jetPhi(iC.consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetPhiLabel"))),
  t_jetE(iC.consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetEnergyLabel"))),
  t_jetHadronFlavour(iC.consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetPartonFlavourLabel"))),
  t_jetPartonFlavour(iC.consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetHadronFlavourLabel"))),
  t_jetCSV(iC.consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetCSVLabel"))),
  t_jetCMVA(iC.consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetCMVALabel"))),
  t_jetCvsB(iC.consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetCvsBLabel"))),
  t_jetCvsL(iC.consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetCvsLLabel"))),
  t_jetJEC(iC.consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetJECLabel"))),
  t_jetnHadEnergy(iC.consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetnHadEnergyLabel"))),
  t_jetnEMEnergy(iC.consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetnEMEnergyLabel"))),
  t_jetcHadEnergy(iC.consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetcHadEnergyLabel"))),
  t_jetcEMEnergy(iC.consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetcEMEnergyLabel"))),
  t_jetcMultip(iC.consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetcMultipLabel"))),
  t_jetnMultip(iC.consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetnMultipLabel"))),
  t_jetY(iC.consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetYLabel"))),
  t_jetArea(iC.consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetAreaLabel"))),
  t_jetMuonEnergy(iC.consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag> ("jetMuonEnergyLabel"))){
    if (m_useTruth){
      t_jetGenPt  = iC.consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetGenPtLabel"));
      t_jetGenEta = iC.consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetGenEtaLabel"));
      t_jetGenPhi = iC.consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetGenPhiLabel"));
      t_jetGenE   = iC.consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetGenELabel"));
      t_jetGenCharge = iC.consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetGenChargeLabel"));
    }
}

Jets::~Jets() {}


std::vector<Jet> Jets::execute(const edm::Event& evt, const objectSelection& obj){
    /* Build the jets */
    evt.getByToken(t_rho,    h_rho);
    evt.getByToken(t_jetY,   h_jetY);
    evt.getByToken(t_jetPt,  h_jetPt);
    evt.getByToken(t_jetEta, h_jetEta);
    evt.getByToken(t_jetPhi, h_jetPhi);
    evt.getByToken(t_jetJEC, h_jetJEC);
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

        jet.cMVAv2     = (h_jetCMVA.product())->at(ijet);

        jet.cMultip    = (h_jetcMultip.product())->at(ijet);
        jet.nMultip    = (h_jetnMultip.product())->at(ijet);
        jet.nEMEnergy  = (h_jetnEMEnergy.product())->at(ijet);
        jet.nHadEnergy = (h_jetnHadEnergy.product())->at(ijet);
        jet.cEMEnergy  = (h_jetcEMEnergy.product())->at(ijet);
        jet.cHadEnergy = (h_jetcHadEnergy.product())->at(ijet);
        jet.muonEnergy = (h_jetcMultip.product())->at(ijet);
        jet.index      = ijet;

        bool passObjSel = obj.pass(jet);
        if (!passObjSel) continue;

        m_jets.push_back(jet);
    }

    return m_jets;
}


std::vector<Jet> Jets::execute_truth(const edm::Event& evt, const objectSelection& obj){
    /* Build Generator jets */
    m_truth_jets.clear();

    evt.getByToken(t_jetGenPt,  h_jetGenPt);
    evt.getByToken(t_jetGenEta, h_jetGenEta);
    evt.getByToken(t_jetGenPhi, h_jetGenPhi);
    evt.getByToken(t_jetGenE,   h_jetGenE);

    for (unsigned ijet=0, size=(h_jetGenPt.product())->size(); ijet<size; ++ijet) {
        Jet jet;
        jet.p4.SetPtEtaPhiE( (h_jetGenPt.product())->at(ijet),  (h_jetGenPt.product())->at(ijet),
                             (h_jetGenPhi.product())->at(ijet), (h_jetGenE.product())->at(ijet));
        jet.charge = (h_jetGenCharge.product())->at(ijet);

       	bool passObjSel	= obj.pass(jet,true);
        if (!passObjSel) continue;

        m_truth_jets.push_back(jet);
    }

    return m_truth_jets;
}

// THE END
