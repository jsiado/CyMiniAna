/*
Created:        --
Last Updated:   18 December 2017

Dan Marley
daniel.edison.marley@cernSPAMNOT.ch
Texas A&M University

-----

Build Electrons from EDMntuples

Uncertainties:
  https://twiki.cern.ch/twiki/bin/viewauth/CMS/EgammaIDRecipesRun2#Electron_efficiencies_and_scale
*/
#include "Analysis/CyMiniAna/interface/Electrons.h"


using namespace edm; 


Electrons::Electrons(edm::ParameterSet const& iConfig, edm::ConsumesCollector && iC) : 
  m_labels(iConfig.getParameter<edm::ParameterSet>("electronLabels")){
    t_elCharge = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("elChargeLabel"));
    t_elDxy = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("elDxyLabel"));
    t_elDz = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("elDzLabel"));
    t_elE = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("elELabel"));
    t_elEta = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("elEtaLabel"));
    t_elHoE = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("elHoELabel"));
    t_elRelIsoEA = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("elRelIsoEALabel"));
    t_elKey = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("elKeyLabel"));
    t_elPhi = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("elPhiLabel"));
    t_elPt = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("elPtLabel"));
    t_eldEtaIn = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("eldEtaInLabel"));
    t_eldEtaInSeed = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("eldEtaInSeedLabel"));
    t_eldPhiIn = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("eldPhiInLabel"));
    t_elfull5x5siee = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("elfull5x5sieeLabel"));
    t_elhasMatchedConVeto = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("elhasMatchedConVetoLabel"));
    t_elvidLoose = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("elvidLooseLabel"));
    t_elvidMedium = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("elvidMediumLabel"));
    t_elvidTight = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("elvidTightLabel"));
    t_elvidVeto = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("elvidVetoLabel"));
    t_elvidHEEP = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("elvidHEEPLabel"));
    t_elmissHits = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("elmissHitsLabel"));
    t_elooEmooP = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("elooEmooPLabel"));
    t_elscEta = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("elscEtaLabel"));

    
    for (const auto& sf : m_listOfSFs){
        TFile* f = TFile::Open(("data/egammaEffi_"+sf+"_EGM2D.root").c_str());  // hard-coded :/
        TH2D* h = (TH2D*)f->Get("EGamma_SF2D");
        TH2D* hist = (TH2D*)h->Clone();
        m_listOfHists[sf] = hist;
    }
}

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

    m_applyIso = obj.applyElectronIsolation();  // access this information from the objectSelection class

    for (unsigned int iel=0, size=(h_elPt.product())->size(); iel<size; ++iel) {
        Electron el;

        el.p4.SetPtEtaPhiE((h_elPt.product())->at(iel), (h_elEta.product())->at(iel),
                           (h_elPhi.product())->at(iel),(h_elE.product())->at(iel));

        el.index     = iel;
        el.charge    = (h_elCharge.product())->at(iel);
        el.vidLoose  = (h_elvidLoose.product())->at(iel);
        el.vidMedium = (h_elvidMedium.product())->at(iel);
        el.vidTight  = (h_elvidTight.product())->at(iel);
        el.vidVeto   = (h_elvidVeto.product())->at(iel);
 
        bool updateElectronID(false);
        if (updateElectronID){
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

            setElectronID(el);
        }

       	bool passObjSel	= obj.pass(el);
        if (!passObjSel) continue;

        // Get SFs
        getSF(el);

        m_electrons.push_back(el);
    }

    return m_electrons;
}


void Electrons::getSF( Electron& el ) const{
    /* Use the Egamma-provided SFs for ID and Reconstruction */
    std::map<std::string,float> sfs;

    float pt  = el.p4.Pt();
    float eta = el.p4.Eta();

    // loop over SFs (defined in header) to access root files
    for (const auto& sf : m_listOfSFs){

        TH2D* h = m_listOfHists.at(sf);

        int xBin = h->GetXaxis()->FindBin( eta );
        int yBin = h->GetYaxis()->FindBin( pt );

        sfs[sf]       = h->GetBinContent(  xBin, yBin );
        sfs[sf+"_UP"] = h->GetBinErrorUp(  xBin, yBin );   // I think Up and Low will be the same?
        sfs[sf+"_DN"] = h->GetBinErrorLow( xBin, yBin );
    }

    // Set attributes of the electron
    el.vetoSF = sfs["veto"];
    el.vetoSF_UP = sfs["veto_UP"];
    el.vetoSF_DN = sfs["veto_DN"];

    el.looseSF = sfs["loose"];
    el.looseSF_UP = sfs["loose_UP"];
    el.looseSF_DN = sfs["loose_DN"];

    el.mediumSF = sfs["medium"];
    el.mediumSF_UP = sfs["medium_UP"];
    el.mediumSF_DN = sfs["medium_DN"];

    el.tightSF = sfs["tight"];
    el.tightSF_UP = sfs["tight_UP"];
    el.tightSF_DN = sfs["tight_DN"];

    el.recoSF = sfs["reco"];
    el.recoSF_UP = sfs["reco_UP"];
    el.recoSF_DN = sfs["reco_DN"];

    return;
}

void Electrons::setElectronID( Electron& el ) const{
    /* Determine electron ID */
    int missHits = el.missHits;
    bool isEB = (std::abs(el.scEta) <= m_barrel_eta);
    bool conv = el.hasMatchedConVeto;
    float Dz  = std::abs(el.Dz);
    float Dxy = std::abs(el.Dxy);
    float HoE = el.HoE;
    float dPhiIn   = std::abs(el.dPhiIn);
    float ooEmooP  = el.ooEmooP;
    float dEtaInSeed  = std::abs(el.dEtaInSeed);
    float full5x5siee = el.full5x5siee;

    // All ID must pass this parameter!
    if (conv) {
        el.vidVeto   = false;
        el.vidLoose  = false;
        el.vidMedium = false;
        el.vidTight  = false;
        return;
    }

    // RelIsoEA requires special treatment in barrel vs endcap
    bool elISO_veto   = electronISO(el,"veto");
    bool elISO_loose  = electronISO(el,"loose");
    bool elISO_medium = electronISO(el,"medium");
    bool elISO_tight  = electronISO(el,"tight");

    // loop over different isolations to reduce code duplication
    // 12 Jan: Need smarter way to do this that is simpler and uses less CPU
    std::map<std::string,bool> isos = { {"veto",elISO_veto}, {"loose",elISO_loose},
                                        {"medium",elISO_medium}, {"tight",elISO_tight} };
    std::map<std::string,bool> ids  = { {"veto",false}, {"loose",false}, {"medium",false}, {"tight",false} };

    for (const auto& iso : isos){
        bool isVETO = (dEtaInSeed < m_dEtaInSeed.at(iso.first).at(isEB)) &&
                  (dPhiIn < m_dPhiIn.at(iso.first).at(isEB)) &&
                  (full5x5siee < m_full5x5siee.at(iso.first).at(isEB)) &&
                  (HoE < m_HoE.at(iso.first).at(isEB)) &&
                  (Dxy < m_Dxy.at(iso.first).at(isEB)) &&
                  (Dz  < m_Dz.at(iso.first).at(isEB))  &&
                  (ooEmooP < m_ooEmooP.at(iso.first).at(isEB)) &&
                  (missHits <= m_missHits.at(iso.first).at(isEB)) &&
                  (iso.second);
        ids.at(iso.first) = isVETO;
    }

    el.vidVeto   = ids.at("veto");
    el.vidLoose  = ids.at("loose");
    el.vidMedium = ids.at("medium");
    el.vidTight  = ids.at("tight");

    return;
}


bool Electrons::electronISO( const Electron& el, const std::string& ID ) const{
    /* Electron Isolation */
    bool elISO(false);
    float RelIsoEA = el.RelIsoEA;

    if ( std::abs(el.scEta) <= m_barrel_eta )
        elISO = (RelIsoEA < m_RelIsoEA.at(ID).at(true));
    else
        elISO = m_applyIso ? (RelIsoEA < m_RelIsoEA.at(ID).at(false)) : true;

    return elISO;
}

// THE END
