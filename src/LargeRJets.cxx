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
  m_useTruth(iConfig.getParameter<edm::InputTag>("useTruth")),
  t_ljetPt(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("ljetPtLabel")),
  t_ljetEta(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("ljetEtaLabel")),
  t_ljetPhi(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("ljetPhiLabel")),
  t_ljetE(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("ljetEnergyLabel")),
  t_ljetCSV(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("ljetCSVLabel")),
  t_ljetCMVA(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("ljetCMVALabel")),
  t_ljetCvsB(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("ljetCvsBLabel")),
  t_ljetCvsL(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("ljetCvsLLabel")),
  t_ljetJEC(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("ljetJECLabel")),
  t_ljetY(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("ljetYLabel")),
  t_ljetArea(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("ljetAreaLabel")),
  t_ljetnHadEnergy(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("ljetnHadEnergyLabel")),
  t_ljetnEMEnergy(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("ljetnEMEnergyLabel")),
  t_ljetcHadEnergy(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("ljetcHadEnergyLabel")),
  t_ljetcEMEnergy(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("ljetcEMEnergyLabel")),
  t_ljetcMultip(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("ljetcMultipLabel")),
  t_ljetnMultip(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("ljetnMultipLabel")),
  t_ljetHadronFlavour(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("ljetPartonFlavourLabel")),
  t_ljetPartonFlavour(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("ljetHadronFlavourLabel")),
  t_ljettau1(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("ljettau1Label")),
  t_ljettau2(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("ljettau2Label")),
  t_ljettau3(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("ljettau3Label")),
  t_ljetPrunedMass(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("ljetPrunedMassLabel")),
  t_ljetTrimmedMass(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("ljetTrimmedMassLabel")),
  t_ljetFilteredMass(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("ljetFilteredMassLabel")),
  t_ljetSoftDropMass(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("ljetSoftDropMassLabel")),
  t_ljet_subjetIdx0(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("ljet_subjetIdx0Label")),
  t_ljet_subjetIdx1(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("ljet_subjetIdx1Label")),
  t_ljet_subjetHadFl(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("ljet_subjetHadronFlavourLabel")),
  t_ljet_subjetPt(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("ljet_subjetPtLabel")),
  t_ljet_subjetEta(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("ljet_subjetEtaLabel")),
  t_ljet_subjetPhi(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("ljet_subjetPhiLabel")),
  t_ljet_subjetEnergy(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("ljet_subjetEnergyLabel")),
  t_ljet_subjetJEC(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("ljet_subjetJECLabel")),
  t_ljet_subjetCSV(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("ljet_subjetCSVLabel")),
  t_ljet_subjetCMVA(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("ljet_subjetCMVALabel"))){
	if (m_useTruth){
		t_ljetGenPt     = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("ljetGenPtLabel"));
		t_ljetGenEta    = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("ljetGenEtaLabel"));
		t_ljetGenPhi    = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("ljetGenPhiLabel"));
		t_ljetGenE      = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("ljetGenELabel"));
		t_ljetGenCharge = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("ljetGenChargeLabel"));
		t_ljet_subjetGenPt     = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("ljet_subjetGenPtLabel"));
		t_ljet_subjetGenEta    = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("ljet_subjetGenEtaLabel"));
		t_ljet_subjetGenPhi    = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("ljet_subjetGenPhiLabel"));
		t_ljet_subjetGenE	   = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("ljet_subjetGenELabel"));
		t_ljet_subjetGenCharge = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("ljet_subjetGenChargeLabel"));
	}
}

LargeRJets::~LargeRJets() {}


std::vector<Ljet> LargeRJets::execute(const edm::Event& evt, const objectSelection& obj){
    /* Build the large-R jets */
    evt.getByToken(t_ljetPt,     h_ljetPt);
    evt.getByToken(t_ljetEta,    h_ljetEta);
    evt.getByToken(t_ljetPhi,    h_ljetPhi);
    evt.getByToken(t_ljetEnergy, h_ljetEnergy);
    evt.getByToken(t_ljetCSV,    h_ljetCSV);
    evt.getByToken(t_ljetCMVA,   h_ljetCMVA);
    evt.getByToken(t_ljetCvsB,   h_ljetCvsB);
    evt.getByToken(t_ljetCvsL,   h_ljetCvsL);
    evt.getByToken(t_ljetJEC,    h_ljetJEC);
    evt.getByToken(t_ljetY,      h_ljetY);
    evt.getByToken(t_ljetArea,   h_ljetArea);
    evt.getByToken(t_ljetnHadEnergy, h_ljetnHadEnergy);
    evt.getByToken(t_ljetnEMEnergy,  h_ljetnEMEnergy);
    evt.getByToken(t_ljetcHadEnergy, h_ljetcHadEnergy);
    evt.getByToken(t_ljetcEMEnergy,  h_ljetcEMEnergy);
    evt.getByToken(t_ljetcMultip,    h_ljetcMultip);
    evt.getByToken(t_ljetnMultip,    h_ljetnMultip);
    evt.getByToken(t_ljetHadronFlavour, h_ljetHadronFlavour);
    evt.getByToken(t_ljetPartonFlavour, h_ljetPartonFlavour);

    for (unsigned ijet=0, size=(h_ljetPt.product())->size(); ijet<size; ++ijet) {
        Jet ljet;

        ljet.p4.SetPtEtaPhiE( (h_ljetPt.product())->at(ijet), (h_ljetEta.product())->at(ijet), 
                              (h_ljetPhi.product())->at(ijet), (h_ljetE.product())->at(ijet));

        if (!obj.pass(ljet))
            continue;

        m_ljets.push_back(jet);
    }

    return m_ljets;
}


std::vector<Jet> LargeRJets::execute_truth(const edm::Event& evt, const objectSelection& obj){
    /* Build Generator large-R jets */
    m_truth_ljets.clear()

    evt.getByToken(t_ljetGenPt,  h_ljetGenPt);
    evt.getByToken(t_ljetGenEta, h_ljetGenEta);
    evt.getByToken(t_ljetGenPhi, h_ljetGenPhi);
    evt.getByToken(t_ljetGenE,   h_ljetGenE);

    for (unsigned ijet=0, size=(h_ljetGenPt.product())->size(); ijet<size; ++ijet) {
        Jet ljet;
        ljet.p4.SetPtEtaPhiE( (h_ljetGenPt.product())->at(ijet),  (h_ljetGenPt.product())->at(ijet),
                              (h_ljetGenPhi.product())->at(ijet), (h_ljetGenE.product())->at(ijet));

        if (!obj.pass(ljet,true))  // check truth-level large-R jet selection
            continue;

        m_truth_ljets.push_back(jet);
    }

    return m_truth_ljets;
}


// THE END