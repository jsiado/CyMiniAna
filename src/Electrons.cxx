/*
Created:        --
Last Updated:   18 December 2017

Dan Marley
daniel.edison.marley@cernSPAMNOT.ch
Texas A&M University

-----

Build Electrons from EDMntuples
*/
#include "Analysis/CyMiniAna/interface/Electrons.h"


using namespace edm; 


Electrons::Electrons(edm::ParameterSet const& iConfig, edm::ConsumesCollector && iC) : 
  t_elCharge(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("elChargeLabel"))),
  t_elDxy(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("elDxyLabel"))),
  t_elDz(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("elDzLabel"))),
  t_elE(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("elELabel"))),
  t_elEta(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("elEtaLabel"))),
  t_elHoE(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("elHoELabel"))),
  t_elRelIsoEA(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("elRelIsoEALabel"))),
  t_elKey(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("elKeyLabel"))),
  t_elPhi(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("elPhiLabel"))),
  t_elPt(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("elPtLabel"))),
  t_eldEtaIn(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("eldEtaInLabel"))),
  t_eldEtaInSeed(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("eldEtaInSeedLabel"))),
  t_eldPhiIn(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("eldPhiInLabel"))),
  t_elfull5x5siee(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("elfull5x5sieeLabel"))),
  t_elvidLoose(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("elvidLooseLabel"))),
  t_elvidMedium(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("elvidMediumLabel"))),
  t_elvidTight(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("elvidTightLabel"))),
  t_elvidVeto(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("elvidVetoLabel"))),
  t_elvidHEEP(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("elvidHEEPLabel"))),
  t_elmissHits(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("elmissHitsLabel"))),
  t_elooEmooP(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("elooEmooPLabel"))),
  t_elscEta(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("elscEtaLabel"))),
  t_elhasMatchedConVeto(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("elhasMatchedConVetoLabel")){}

Electrons::~Electrons() {}


std::vector<Electron> Electrons::execute(const edm::Event& evt, const objectSelection& obj){
    /* Build the electrons */
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

        el.p4.SetPtEtaPhiE((h_elPt.product())->at(iel), (h_elEta.product())->at(iel),
                           (h_elPhi.product())->at(iel),(h_elE.product())->at(iel),);

        el.charge    = (h_elCharge.product())->at(iel);
        el.vidLoose  = (h_elvidLoose.product())->at(iel);
        el.vidMedium = (h_elvidMedium.product())->at(iel);
        el.vidTight  = (h_elvidTight.product())->at(iel);
        el.vidVeto   = (h_elvidVeto.product())->at(iel);
 
        el.Dz  = (h_elDz.product())->at(iel);
        el.Dxy = (h_elDxy.product())->at(iel);
        el.HoE = (h_elHoE.product())->at(iel);
        el.scEta  = (h_elscEta.product())->at(iel);
        el.dPhiIn = (h_eldPhiIn.product())->at(iel);
        el.ooEmooP  = (h_elooEmooP.product())->at(iel);
        el.missHits = (h_elmissHits.product())->at(iel);
        el.RelIsoEA = (h_elRelIsoEA.product())->at(iel);
        el.dEtaInSeed  = (h_eldEtaInSeed.product())->at(iel);
        el.full5x5siee = (h_elfull5x5siee.product())->at(iel);
        el.hasMatchedConVeto = (h_elhasMatchedConVeto.product())->at(iel);

        if (!obj.pass(el)) continue;

        m_electrons.push_back(el);
    }

    return m_electrons;
}


// THE END