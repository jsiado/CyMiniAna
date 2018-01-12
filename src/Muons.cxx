/*
Created:        --
Last Updated:   18 December 2017

Dan Marley
daniel.edison.marley@cernSPAMNOT.ch
Texas A&M University

-----

Build Electrons from EDMntuples
*/
#include "Analysis/CyMiniAna/interface/Muons.h"


using namespace edm; 


Muons::Muons(edm::ParameterSet const& iConfig, edm::ConsumesCollector && iC) : 
  t_muDxy(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("muDxyLabel"))),
  t_muDz(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("muDzLabel"))),
  t_muE(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("muELabel"))),
  t_muEta(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("muEtaLabel"))),
  t_muPhi(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("muPhiLabel"))),
  t_muPt(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("muPtLabel"))),
  t_muKey(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("muKeyLabel"))),
  t_muIso04(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("muIso04Label"))),
  t_muCharge(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("muChargeLabel"))),
  t_muSumPUPt(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("muSumPUPtLabel"))),
  t_muSumPhotonPt(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("muSumPhotonPtLabel"))),
  t_muGlbTrkNormChi2(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("muGlbTrkNormChi2Label"))),
  t_muInTrkNormChi2(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("muInTrkNormChi2Label"))),
  t_muIsGlobalMuon(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("muIsGlobalMuonLabel"))),
  t_muIsLooseMuon(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("muIsLooseMuonLabel"))),
  t_muIsPFMuon(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("muIsPFMuonLabel"))),
  t_muIsSoftMuon(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("muIsSoftMuonLabel"))),
  t_muIsTightMuon(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("muIsTightMuonLabel"))),
  t_muIsTrackerMuon(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("muIsTrackerMuonLabel"))),
  t_muNumberOfPixelLayers(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("muNumberOfPixelLayersLabel"))),
  t_muNumberTrackerLayers(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("muNumberTrackerLayersLabel"))),
  t_muNumberValidMuonHits(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("muNumberValidMuonHitsLabel"))),
  t_muNumberValidPixelHits(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("muNumberValidPixelHitsLabel"))),
  t_muSumChargedHadronPt(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("muSumChargedHadronPtLabel"))),
  t_muSumNeutralHadronPt(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("muSumNeutralHadronPtLabel"))),
  t_muNumberMatchedStations(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("muNumberMatchedStationsLabel"))),
  t_muNumberOfValidTrackerHits(consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("muNumberOfValidTrackerHitsLabel"))){
    m_muPtMin     = iConfig.getParameter<float>("muPtMin");
    m_muAbsEtaMax = iConfig.getParameter<float>("muAbsEtaMax");
    m_muIsoMin    = iConfig.getParameter<float>("muIsoMin");
    m_muIsoMax    = iConfig.getParameter<float>("muIsoMax");

    if (m_useTruth){
        t_muGenE   = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("muGenELabel"))),
        t_muGenEta = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("muGenEtaLabel"))),
        t_muGenPhi = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("muGenPhiLabel"))),
        t_muGenPt  = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("muGenPtLabel"))),
        t_muGenCharge = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("muGenChargeLabel"))),
    }
}

Muons::~Muons() {}


std::vector<Muon> Muons::execute(const edm::Event& evt, const objectSelection& obj){
    /* Build Muons */
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
        float muPt  = (h_muPt.product())->at(imu);
        float muEta = (h_muEta.product())->at(imu);
        float muIso = (h_muIso04.product())->at(imu);

        bool passMuId(false);
        bool isLooseMu((h_muIsLooseMuon.product())->at(imu) > 0);
        bool isTightMu((h_muIsTightMuon.product())->at(imu) > 0);

        bool passMuISO(false);
        if ( muIso >= m_muIsoMin && muIso < m_muIsoMax ) passMuISO = true;

        // kinematics, ID, and isolation
        if ( muPt < m_muPtMin || abs(muEta) > m_muAbsEtaMax || !passMuID || !passMuISO )
             continue;

        Muon mu;
        mu.p4.SetPtEtaPhiE( muPt, muEta, (h_muPhi.product())->at(imu), (h_muE.product())->at(imu) );
        /* ... */

        if (!obj.pass(mu)) continue;

        m_muons.push_back(muon) ;
    }

    return m_muons;
}



std::vector<Muon> Muons::execute_truth(const edm::Event& evt, const objectSelection& obj){
    /* Build Generator Muons */
    m_truth_muons.clear();

    evt.getByToken(t_muGenE,   h_muGenE);
    evt.getByToken(t_muGenEta, h_muGenEta);
    evt.getByToken(t_muGenPhi, h_muGenPhi);
    evt.getByToken(t_muGenPt,  h_muGenPt);
    evt.getByToken(t_muGenCharge, h_muGenCharge);

    for (unsigned int imu=0, size=(h_muGenMuonPt.product())->size(); imu<size; ++imu) {
        Muon mu;
        mu.p4.SetPtEtaPhiE( (h_muGenMuonPt.product())->at(imu),  (h_muGenMuonEta.product())->at(imu),
                            (h_muGenMuonPhi.product())->at(imu), (h_muGenMuonE.product())->at(imu) );
        mu.charge = (h_muGenMuonCharge.product())->at(imu);

        if (!obj.pass(mu,true))  // check truth-level muon selection
            continue;

        m_truth_muons.push_back(mu);
    }

    return m_truth_muons;
}

// THE END
