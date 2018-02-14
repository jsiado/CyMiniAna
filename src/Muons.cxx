/*
Created:        --
Last Updated:   18 December 2017

Dan Marley
daniel.edison.marley@cernSPAMNOT.ch
Texas A&M University

-----

Build Muons from EDMntuples
*/
#include "Analysis/CyMiniAna/interface/Muons.h"


using namespace edm; 


Muons::Muons(edm::ParameterSet const& iConfig, edm::ConsumesCollector && iC) : 
  m_labels(iConfig.getParameter<edm::ParameterSet>("muonLabels")){
    m_useTruth = iConfig.getParameter<bool>("useTruth");
    t_muPt = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("muPtLabel"));
    t_muEta = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("muEtaLabel"));
    t_muPhi = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("muPhiLabel"));
    t_muE = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("muELabel"));
    t_muCharge = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("muChargeLabel"));
    t_muDxy = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("muDxyLabel"));
    t_muDz = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("muDzLabel"));
    t_muGlbTrkNormChi2 = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("muGlbTrkNormChi2Label"));
    t_muInTrkNormChi2 = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("muInTrkNormChi2Label"));
    t_muIsGlobalMuon = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("muIsGlobalMuonLabel"));
    t_muIsLooseMuon = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("muIsLooseMuonLabel"));
    t_muIsPFMuon = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("muIsPFMuonLabel"));
    t_muIsSoftMuon = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("muIsSoftMuonLabel"));
    t_muIsTightMuon = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("muIsTightMuonLabel"));
    t_muIsTrackerMuon = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("muIsTrackerMuonLabel"));
    t_muIso04 = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("muIso04Label"));
    t_muKey = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("muKeyLabel"));
    t_muNumberMatchedStations = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("muNumberMatchedStationsLabel"));
    t_muNumberOfPixelLayers = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("muNumberOfPixelLayersLabel"));
    t_muNumberOfValidTrackerHits = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("muNumberOfValidTrackerHitsLabel"));
    t_muNumberTrackerLayers = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("muNumberTrackerLayersLabel"));
    t_muNumberValidMuonHits = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("muNumberValidMuonHitsLabel"));
    t_muNumberValidPixelHits = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("muNumberValidPixelHitsLabel"));
    t_muSumChargedHadronPt = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("muSumChargedHadronPtLabel"));
    t_muSumNeutralHadronPt = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("muSumNeutralHadronPtLabel"));
    t_muSumPUPt = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("muSumPUPtLabel"));
    t_muSumPhotonPt = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("muSumPhotonPtLabel"));
    if (m_useTruth){
        t_muGenE   = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("muGenELabel"));
        t_muGenEta = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("muGenEtaLabel"));
        t_muGenPhi = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("muGenPhiLabel"));
        t_muGenPt  = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("muGenPtLabel"));
        t_muGenCharge = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("muGenChargeLabel"));
    }

    // Get histograms & store them for later
    m_listOfHists.clear();

    // ID
    // https://twiki.cern.ch/twiki/bin/viewauth/CMS/SWGuideMuonIdRun2#Short_Term_Instructions_for_Mori
    TFile* f = TFile::Open("data/muonEffi_ID_BCDEF.root");
    TH2D* h  = (TH2D*)f->Get("MC_NUM_MediumID2016_DEN_genTracks_PAR_pt_eta/pt_abseta_ratio");
    m_listOfHists["ID_BCDEF"] = h;

    f = TFile::Open("data/muonEffi_ID_GH.root");
    h = (TH2D*)f->Get("MC_NUM_MediumID_DEN_genTracks_PAR_pt_eta/pt_abseta_ratio");
    m_listOfHists["ID_GH"] = h;

    // ISO
    f = TFile::Open("data/muonEffi_ISO_BCDEF.root");
    h = (TH2D*)f->Get("LooseISO_MediumID_pt_eta/pt_abseta_ratio");
    m_listOfHists["ISO_BCDEF"] = h;

    f = TFile::Open("data/muonEffi_ISO_GH.root");
    h = (TH2D*)f->Get("LooseISO_MediumID_pt_eta/pt_abseta_ratio");
    m_listOfHists["ISO_GH"] = h;

    // TRIG
    f = TFile::Open("data/muonEffi_TRIG_RunBtoF.root");
    h = (TH2D*)f->Get("IsoMu24_OR_IsoTkMu24_PtEtaBins/pt_abseta_ratio");
    m_listOfHists["TRIG_BCDEF"] = h;

    f = TFile::Open("data/muonEffi_TRIG_Period4.root");
    h = (TH2D*)f->Get("IsoMu24_OR_IsoTkMu24_PtEtaBins/pt_abseta_ratio");
    m_listOfHists["TRIG_GH"] = h;

    // TRACK
    f = TFile::Open("data/muonEffi_TRACK_BCDEFGH.root");
    m_trackHisto = (TGraphAsymmErrors*)f->Get("eff_eta3_dr030e030_corr");
}

Muons::~Muons() {}


std::vector<Muon> Muons::execute(const edm::Event& evt, const objectSelection& obj){
    /* Build Muons */
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
        Muon mu;
        mu.p4.SetPtEtaPhiE( (h_muPt.product())->at(imu), (h_muEta.product())->at(imu), 
                            (h_muPhi.product())->at(imu), (h_muE.product())->at(imu) );
        mu.iso04 = (h_muIso04.product())->at(imu);
        mu.loose = ((h_muIsLooseMuon.product())->at(imu) > 0);
        mu.tight = ((h_muIsTightMuon.product())->at(imu) > 0);

       	bool passObjSel	= obj.pass(mu);
        if (!passObjSel) continue;

        // ID, ISO, TRIG, & TRACK SFs
        getSF(mu);

        m_muons.push_back(mu);
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

    for (unsigned int imu=0, size=(h_muGenPt.product())->size(); imu<size; ++imu) {
        Muon mu;
        mu.p4.SetPtEtaPhiE( (h_muGenPt.product())->at(imu),  (h_muGenEta.product())->at(imu),
                            (h_muGenPhi.product())->at(imu), (h_muGenE.product())->at(imu) );
        mu.charge = (h_muGenCharge.product())->at(imu);

       	bool passObjSel	= obj.pass(mu,true);
        if (!passObjSel) continue;

        m_truth_muons.push_back(mu);
    }

    return m_truth_muons;
}


void Muons::getSF( Muon& mu ){
    /* Retrieve SFs from ROOT files 
       -- Accessed from https://twiki.cern.ch/twiki/bin/view/CMS/MuonWorkInProgressAndPagResults#Results_on_the_full_2016_data
       -- 9 February 2018

       Trigger SFs are centrally-provided (using standard triggers)
       Use luminosity to weight SFs across run periods
    */
    float lumiBF = m_lumi_2016["b"] + m_lumi_2016["c"] + m_lumi_2016["d"] + m_lumi_2016["e"] + m_lumi_2016["f"];
    float lumiGH = m_lumi_2016["g"] + m_lumi_2016["h_1"] + m_lumi_2016["h_2"];

    float lumitotal = lumiBF + lumiGH;
    lumiBF /= lumitotal;
    lumiGH /= lumitotal;

    std::map<std::string,float> valuesBF;
    std::map<std::string,float> valuesGH;

    // ID :: LooseISO & MediumID2016 (BCDEF) + MediumID (GH)
    // https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideMuonIdRun2#Short_Term_Instructions_for_Mori
    valuesBF = getSF_values( "ID_BCDEF", mu.p4 );
    valuesGH = getSF_values( "ID_GH", mu.p4 );

    mu.mediumID_SF    = valuesBF["sf"]*lumiBF + valuesGH["sf"]*lumiGH;
    mu.mediumID_SF_UP = valuesBF["sf_up"]*lumiBF + valuesGH["sf_dn"]*lumiGH;
    mu.mediumID_SF_DN = valuesBF["sf_up"]*lumiBF + valuesGH["sf_dn"]*lumiGH;

    // ISO
    valuesBF = getSF_values( "ISO_BCDEF", mu.p4 );
    valuesGH = getSF_values( "ISO_GH", mu.p4 );

    mu.looseISO_SF    = valuesBF["sf"]*lumiBF + valuesGH["sf"]*lumiGH;
    mu.looseISO_SF_UP = valuesBF["sf_up"]*lumiBF + valuesGH["sf_dn"]*lumiGH;
    mu.looseISO_SF_DN = valuesBF["sf_up"]*lumiBF + valuesGH["sf_dn"]*lumiGH;

    // TRIG
    valuesBF = getSF_values( "TRIG_BCDEF", mu.p4 );
    valuesGH = getSF_values( "TRIG_GH", mu.p4 );

    mu.trigger_SF    = valuesBF["sf"]*lumiBF + valuesGH["sf"]*lumiGH;
    mu.trigger_SF_UP = valuesBF["sf_up"]*lumiBF + valuesGH["sf_dn"]*lumiGH;
    mu.trigger_SF_DN = valuesBF["sf_up"]*lumiBF + valuesGH["sf_dn"]*lumiGH;

    // TRACK -- only 1 SF for the entire 2016 dataset
    int x = m_trackHisto->GetXaxis()->FindBin( mu.p4.Eta() );

    mu.track_SF    = m_trackHisto->GetErrorY(x);
    mu.track_SF_UP = m_trackHisto->GetErrorYhigh(x);
    mu.track_SF_DN = m_trackHisto->GetErrorYlow(x);

    return;
}


std::map<std::string,float> Muons::getSF_values( const std::string& histname, const TLorentzVector& p ){
    /* Quickly retrieve the SF nominal and uncertainty values (cleans up the getSF() function above) */
    std::map<std::string,float> values;

    TH2D* h = m_listOfHists.at(histname);

    // Check pT bounds
    int x(0);
    if (p.Pt() > h->GetXaxis()->GetBinUpEdge(h->GetNbinsX()) )
        x = h->GetNbinsX();
    else
        x = h->GetXaxis()->FindBin( p.Pt() );

    int y = h->GetYaxis()->FindBin( std::abs(p.Eta()) );

    values["sf"] = h->GetBinContent(x,y);
    values["sf_up"] = h->GetBinErrorUp(x,y);
    values["sf_dn"] = h->GetBinErrorLow(x,y);

    return values;
}

// THE END
